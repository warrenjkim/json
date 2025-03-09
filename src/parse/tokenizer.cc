#include "warren/internal/parse/tokenizer.h"

#include <cctype>
#include <cstdio>
#include <optional>
#include <string>
#include <string_view>
#include <utility>  // pair

#include "warren/internal/dsa/queue.h"
#include "warren/internal/parse/token.h"

namespace json {

std::optional<json::dsa::Queue<Token>> Tokenizer::tokenize(
    std::string_view json) {
  json::dsa::Queue<Token> tokens;

  size_t index = strip_whitespace(json, 0);
  if (index >= json.length()) {
    return tokens;
  }

  std::optional<size_t> new_index;
  switch (json[index]) {
    case '{':
      new_index = tokenize_object(json, index, tokens);
      break;
    case '[':
      new_index = tokenize_array(json, index, tokens);
      break;
    default:
      return std::nullopt;
  }

  if (!new_index) {
    return std::nullopt;
  }

  index = strip_whitespace(json, *new_index);
  if (index < json.length()) {
    return std::nullopt;
  }

  tokens.emplace("\0", TokenType::END_OF_JSON);
  return tokens;
}

std::optional<size_t> Tokenizer::tokenize_object(
    const std::string_view json, size_t index, json::dsa::Queue<Token>& tokens,
    const size_t indent_level) {
  char c = json[index];
  tokens.emplace(c, TokenType::OBJECT_START);

  index = strip_whitespace(json, index + 1);
  if (index >= json.length()) {
    return std::nullopt;
  }

  if (json[index] == '}') {
    tokens.emplace('}', TokenType::OBJECT_END);
    return strip_whitespace(json, index + 1);
  }

  while (true) {
    std::optional<size_t> new_index =
        tokenize_key_value(json, index, tokens, indent_level + 1);
    if (!new_index) {
      return std::nullopt;
    }

    index = strip_whitespace(json, *new_index);
    if (index >= json.length()) {
      return std::nullopt;
    }

    c = json[index];
    if (c == '}') {
      tokens.emplace(c, TokenType::OBJECT_END);
      return strip_whitespace(json, index + 1);
    }

    if (c == ',') {
      tokens.emplace(c, TokenType::COMMA);
    } else {
      return std::nullopt;
    }

    index = strip_whitespace(json, index + 1);
    if (index >= json.length()) {
      return std::nullopt;
    }
  }
}

std::optional<size_t> Tokenizer::tokenize_array(const std::string_view json,
                                                size_t index,
                                                json::dsa::Queue<Token>& tokens,
                                                const size_t indent_level) {
  char c = json[index];
  tokens.emplace(c, TokenType::ARRAY_START);

  index = strip_whitespace(json, index + 1);
  if (index >= json.length()) {
    return std::nullopt;
  }

  if (json[index] == ']') {
    tokens.emplace(']', TokenType::ARRAY_END);
    return strip_whitespace(json, index + 1);
  }

  while (true) {
    std::optional<size_t> new_index =
        tokenize_value(json, index, tokens, indent_level + 1);
    if (!new_index) {
      return std::nullopt;
    }

    index = strip_whitespace(json, *new_index);
    if (index >= json.length()) {
      return std::nullopt;
    }

    c = json[index];
    if (c == ']') {
      tokens.emplace(c, TokenType::ARRAY_END);
      return strip_whitespace(json, index + 1);
    }

    if (c == ',') {
      tokens.emplace(c, TokenType::COMMA);
    } else {
      return std::nullopt;
    }

    index = strip_whitespace(json, index + 1);
    if (index >= json.length()) {
      return std::nullopt;
    }
  }
}

std::optional<size_t> Tokenizer::tokenize_value(const std::string_view json,
                                                size_t index,
                                                json::dsa::Queue<Token>& tokens,
                                                const size_t indent_level) {
  char c = json[index];
  if (c == '"') {
    return tokenize_string(json, index, tokens, indent_level);
  } else if (c == '{') {
    return tokenize_object(json, index, tokens, indent_level);
  } else if (c == '[') {
    return tokenize_array(json, index, tokens, indent_level);
  } else if (isdigit(c) || c == '-') {
    return tokenize_number(json, index, tokens, indent_level);
  } else if (tolower(c) == 't' || tolower(c) == 'f' || tolower(c) == 'n') {
    return tokenize_logical_value(json, index, tokens, indent_level);
  }

  return std::nullopt;
}

std::optional<size_t> Tokenizer::tokenize_string(
    const std::string_view json, size_t index, json::dsa::Queue<Token>& tokens,
    const size_t indent_level) {
  char c = json[index++];
  tokens.emplace(c, TokenType::QUOTE);

  std::string token = "";
  while (true) {
    if (index >= json.length()) {
      return std::nullopt;
    }

    c = json[index++];
    if (c == '"') {
      tokens.emplace(token, TokenType::STRING);
      tokens.emplace(c, TokenType::QUOTE);

      return strip_whitespace(json, index);
    }

    if (c == '\\') {
      auto ctrl_char_result =
          tokenize_control_character(json, index, tokens, indent_level + 1);
      if (!ctrl_char_result) {
        return std::nullopt;
      }

      index = ctrl_char_result->first;
      token += ctrl_char_result->second;
    } else {
      token += c;
    }
  }

  return std::nullopt;
}

std::optional<size_t> Tokenizer::tokenize_number(
    const std::string_view json, size_t index, json::dsa::Queue<Token>& tokens,
    const size_t indent_level) {
  std::string token = "";
  if (json[index] == '-') {
    token += '-';
    if (++index >= json.length() || !isdigit(json[index])) {
      return std::nullopt;
    }
  }

  auto [new_index, integer] = tokenize_integer(json, index, indent_level + 1);
  token += integer;
  index = strip_whitespace(json, new_index);

  if (index < json.length() && json[index] == '.') {
    token += '.';
    auto [new_index, fraction] =
        tokenize_integer(json, index + 1, indent_level + 1);
    if (fraction.empty()) {
      return std::nullopt;
    }

    token += fraction;
    index = strip_whitespace(json, new_index);
  }

  if (index < json.length() && tolower(json[index]) == 'e') {
    token += json[index++];
    if (index < json.length() && (json[index] == '+' || json[index] == '-')) {
      token += json[index++];
    }

    auto [new_index, exponent] =
        tokenize_integer(json, index, indent_level + 1);
    if (exponent.empty()) {
      return std::nullopt;
    }

    token += exponent;
    index = strip_whitespace(json, new_index);
  }

  if (token.front() == '0' && token.length() > 1 &&
      (token.at(1) != '.' && tolower(token.at(1)) != 'e')) {
    return std::nullopt;
  }

  tokens.emplace(token, TokenType::NUMBER);
  return index;
}

std::optional<size_t> Tokenizer::tokenize_key_value(
    const std::string_view json, size_t index, json::dsa::Queue<Token>& tokens,
    const size_t indent_level) {
  std::optional<size_t> new_index =
      tokenize_string(json, index, tokens, indent_level + 1);
  if (!new_index) {
    return std::nullopt;
  }

  index = strip_whitespace(json, *new_index);
  if (index >= json.length()) {
    return std::nullopt;
  }

  char c = json[index];
  if (c == ':') {
    tokens.emplace(c, TokenType::COLON);
  } else {
    return std::nullopt;
  }

  index = strip_whitespace(json, index + 1);
  if (index >= json.length()) {
    return std::nullopt;
  }

  new_index = tokenize_value(json, index, tokens, indent_level + 1);
  if (!new_index) {
    return std::nullopt;
  }

  return strip_whitespace(json, *new_index);
}

std::pair<size_t, std::string> Tokenizer::tokenize_integer(
    const std::string_view json, size_t index, const size_t indent_level) {
  std::string token = "";
  char c = json[index];
  while (isdigit(c)) {
    token += c;
    if (++index >= json.length()) {
      break;
    }

    c = json[index];
  }

  return std::make_pair(index, token);
}

std::optional<size_t> Tokenizer::tokenize_logical_value(
    const std::string_view json, size_t index, json::dsa::Queue<Token>& tokens,
    const size_t indent_level) {
  switch (json[index]) {
    case 't':
      if (json.substr(index, 4) == "true") {
        tokens.emplace("true", TokenType::BOOLEAN);
      } else {
        return std::nullopt;
      }

      return strip_whitespace(json, index + 4);
    case 'f':
      if (json.substr(index, 5) == "false") {
        tokens.emplace("false", TokenType::BOOLEAN);
      } else {
        return std::nullopt;
      }

      return strip_whitespace(json, index + 5);
    case 'n':
      if (json.substr(index, 4) == "null") {
        tokens.emplace("null", TokenType::JSON_NULL);
      } else {
        return std::nullopt;
      }

      return strip_whitespace(json, index + 4);
  }

  return std::nullopt;
}

std::optional<std::pair<size_t, std::string>>
Tokenizer::tokenize_control_character(const std::string_view json, size_t index,
                                      json::dsa::Queue<Token>& tokens,
                                      const size_t indent_level) {
  if (index >= json.length()) {
    return std::nullopt;
  }

  char c = json[index];
  std::string token = "\\";
  const size_t HEX_CODE_LENGTH = 4;
  switch (c) {
    case 'u':
      token += c;
      if (++index >= json.length()) {
        return std::nullopt;
      }

      for (size_t i = 0; i < HEX_CODE_LENGTH; i++) {
        char hex_digit = json[index++];
        if (!is_hex(hex_digit) || index >= json.length()) {
          return std::nullopt;
        }

        token += hex_digit;
      }

      return std::make_pair(index, token);
    case '"':
    case '\\':
    case '/':
    case 'b':
    case 'f':
    case 'n':
    case 'r':
    case 't':
      token += c;
      return std::make_pair(index + 1, token);
    default:
      return std::nullopt;
  }

  return std::nullopt;
}

size_t Tokenizer::strip_whitespace(std::string_view json, size_t index) {
  while (index < json.length() && isspace(json[index])) {
    index++;
  }

  return index;
}

bool Tokenizer::is_hex(const char c) {
  switch (c) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
      return true;
    default:
      return false;
  }
}

}  // namespace json
