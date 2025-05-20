#include "warren/internal/parse/parser.h"

#include <cmath>  // pow
#include <cstddef>
#include <cstdint>
#include <exception>
#include <optional>
#include <string>
#include <string_view>

#include "warren/internal/dsa/numeric.h"
#include "warren/internal/dsa/pair.h"
#include "warren/internal/dsa/queue.h"
#include "warren/internal/nodes/node.h"
#include "warren/internal/nodes/object.h"
#include "warren/internal/parse/lexer.h"
#include "warren/internal/parse/token.h"
#include "warren/internal/parse/tokenizer.h"
#include "warren/json/exception.h"

namespace json {

nodes::Node* Parser::parse(const std::string_view json) {
  std::optional<json::dsa::Queue<Token>> tokens = Tokenizer::tokenize(json);
  if (!tokens || tokens->empty()) {
    return nullptr;
  }

  return parse(*tokens);
}

nodes::Node* Parser::parse(json::dsa::Queue<Token>& tokens) {
  if (tokens.empty()) {
    return nullptr;
  }

  switch (tokens.front()->type) {
    case TokenType::OBJECT_START:
      return parse_object(tokens);
    case TokenType::ARRAY_START:
      return parse_array(tokens);
    default:
      return nullptr;
  }
}

nodes::Object* Parser::parse_object(json::dsa::Queue<Token>& tokens,
                                    const size_t indent_level) {
  if (!expect_next(tokens, Token('{', TokenType::OBJECT_START))) {
    return nullptr;
  }

  nodes::Object* object = new nodes::Object();
  if (tokens.front() == Token('}', TokenType::OBJECT_END)) {
    tokens.dequeue();
    return object;
  }

  while (true) {
    if (tokens.empty()) {
      delete object;
      return nullptr;
    }

    auto key_value = parse_key_value(tokens);
    if (!key_value) {
      delete object;
      return nullptr;
    }

    object->insert(key_value->first, key_value->second);

    std::optional<Token> token = tokens.dequeue();
    if (!token) {
      delete object;
      return nullptr;
    }

    switch (token->type) {
      case TokenType::OBJECT_END:
        if (token->value != "}") {
          delete object;
          return nullptr;
        }

        return object;
      case TokenType::COMMA:
        if (token->value != ",") {
          delete object;
          return nullptr;
        }

        break;
      default:
        delete object;
        return nullptr;
    }
  }

  return nullptr;
}

nodes::Array* Parser::parse_array(json::dsa::Queue<Token>& tokens,
                                  const size_t indent_level) {
  if (!expect_next(tokens, Token('[', TokenType::ARRAY_START))) {
    return nullptr;
  }

  nodes::Array* array = new nodes::Array();
  if (tokens.front() == Token(']', TokenType::ARRAY_END)) {
    tokens.dequeue();
    return array;
  }

  while (true) {
    if (tokens.empty()) {
      delete array;
      return nullptr;
    }

    nodes::Node* value = parse_value(tokens);
    if (!value) {
      delete array;
      return nullptr;
    }

    array->push_back(value);

    std::optional<Token> token = tokens.dequeue();
    if (!token) {
      delete array;
      return nullptr;
    }

    switch (token->type) {
      case TokenType::ARRAY_END:
        if (token->value != "]") {
          delete array;
          return nullptr;
        }

        return array;
      case TokenType::COMMA:
        if (token->value != ",") {
          delete array;
          return nullptr;
        }

        break;
      default:
        delete array;
        return nullptr;
    }
  }

  return nullptr;
}

nodes::Node* Parser::parse_value(json::dsa::Queue<Token>& tokens,
                                 const size_t indent_level) {
  if (tokens.empty()) {
    return nullptr;
  }

  switch (tokens.front()->type) {
    case TokenType::QUOTE:
      return parse_string(tokens, indent_level + 1);
    case TokenType::OBJECT_START:
      return parse_object(tokens, indent_level + 1);
    case TokenType::ARRAY_START:
      return parse_array(tokens, indent_level + 1);
    case TokenType::NUMBER:
      return parse_number(tokens, indent_level + 1);
    case TokenType::BOOLEAN:
      return parse_boolean(tokens, indent_level + 1);
    case TokenType::JSON_NULL:
      return parse_null(tokens, indent_level + 1);
    default:
      return nullptr;
  }
}

nodes::String* Parser::parse_string(json::dsa::Queue<Token>& tokens,
                                    const size_t indent_level) {
  if (!expect_next(tokens, Token('"', TokenType::QUOTE))) {
    return nullptr;
  }

  std::optional<Token> token = tokens.dequeue();
  if (!token || token->type != TokenType::STRING) {
    return nullptr;
  }

  nodes::String* string = new nodes::String(token->value);

  if (!expect_next(tokens, Token('"', TokenType::QUOTE))) {
    delete string;
    return nullptr;
  }

  return string;
}

nodes::Number* Parser::parse_number(json::dsa::Queue<Token>& tokens,
                                    const size_t indent_level) {
  std::string number_string = tokens.dequeue()->value;
  size_t exponent_index = number_string.find_first_of("eE");
  double base = 0.0;
  int exponent = 0;
  bool intgr = true;
  try {
    if (exponent_index != std::string::npos) {
      intgr = false;
      base = std::stod(number_string.substr(0, exponent_index));
      exponent = std::stoi(number_string.substr(exponent_index + 1));
    } else {
      if (number_string.find_first_of(".") != std::string::npos) {
        intgr = false;
      }

      base = std::stod(number_string);
    }

    return new nodes::Number(intgr
                                 ? dsa::Numeric(int64_t(base))
                                 : dsa::Numeric(base * std::pow(10, exponent)));
  } catch (const std::exception& e) {
    return nullptr;
  }
}

std::optional<dsa::Pair<std::string, nodes::Node*>> Parser::parse_key_value(
    json::dsa::Queue<Token>& tokens, const size_t indent_level) {
  nodes::String* string = parse_string(tokens, indent_level);
  if (!string) {
    delete string;
    return std::nullopt;
  }

  std::string key = string->get();
  delete string;

  if (!expect_next(tokens, Token(':', TokenType::COLON))) {
    return std::nullopt;
  }

  nodes::Node* value = parse_value(tokens, indent_level + 1);
  if (!value) {
    delete value;
    return std::nullopt;
  }

  return dsa::Pair(key, value);
}

nodes::Boolean* Parser::parse_boolean(json::dsa::Queue<Token>& tokens,
                                      const size_t indent_level) {
  std::optional<Token> token = tokens.dequeue();
  if (token->value != "true" && token->value != "false") {
    return nullptr;
  }

  return new nodes::Boolean(token->value == "true" ? true : false);
}

nodes::Null* Parser::parse_null(json::dsa::Queue<Token>& tokens,
                                const size_t indent_level) {
  if (!expect_next(tokens, Token("null", TokenType::JSON_NULL))) {
    return nullptr;
  }

  return new nodes::Null();
}

const bool Parser::expect_next(json::dsa::Queue<Token>& tokens,
                               const Token& expected) {
  std::optional<Token> token = tokens.dequeue();

  return token && (*token == expected);
}

}  // namespace json

namespace {

uint32_t to_code_point(const char c1, const char c2, const char c3,
                       const char c4) {
  auto to_uint = [](const char c) -> uint32_t {
    if (c >= '0' && c <= '9') {
      return c - '0';
    } else if (c >= 'A' && c <= 'F') {
      return c - 'A' + 10;
    } else if (c >= 'a' && c <= 'f') {
      return c - 'a' + 10;
    }

    return 0;
  };

  uint32_t code_point = 0;
  code_point = (code_point << 4) | to_uint(c1);
  code_point = (code_point << 4) | to_uint(c2);
  code_point = (code_point << 4) | to_uint(c3);
  code_point = (code_point << 4) | to_uint(c4);

  return code_point;
}

// https://www.ietf.org/rfc/rfc3629.txt
void emit_utf8(uint32_t code_point, std::string& res) {
  if (code_point > 0x10FFFF) {
    throw json::ParseException("Code point out of Unicode range (> 0x10FFFF)");
  } else if (0xD800 <= code_point && code_point <= 0xDFFF) {
    throw json::ParseException(
        "Code point in surrogate range [0xD800, 0xDFFF]");
  }

  if (code_point < 0x80) {
    res += (char)(code_point);
  } else if (code_point < 0x800) {
    res += (char)(0xC0 | (code_point >> 6));
    res += (char)(0x80 | (code_point & 0x3F));
  } else if (code_point < 0x10000) {
    res += (char)(0xE0 | (code_point >> 12));
    res += (char)(0x80 | ((code_point >> 6) & 0x3F));
    res += (char)(0x80 | (code_point & 0x3F));
  } else {
    res += (char)(0xF0 | (code_point >> 18));
    res += (char)(0x80 | ((code_point >> 12) & 0x3F));
    res += (char)(0x80 | ((code_point >> 6) & 0x3F));
    res += (char)(0x80 | (code_point & 0x3F));
  }
}

void resolve_unicode_sequences(const std::string& s, std::string& res) {
  res.reserve(s.length());
  size_t i = 0;
  size_t j = 0;
  while (j < s.length()) {
    if (s[j] == '\\' && s[j + 1] == 'u') {
      res.append(s, i, j - i);
      j += 6;

      uint32_t code_point =
          to_code_point(s[j - 4], s[j - 3], s[j - 2], s[j - 1]);

      // Section 3.8 Surrogates
      // https://www.unicode.org/versions/Unicode15.0.0/ch03.pdf
      // 0xD800 <= high-surrogate code point <= 0xDBFF
      uint16_t high_surrogate_cp = code_point;
      if (0xD800 <= high_surrogate_cp && high_surrogate_cp <= 0xDBFF) {
        j += 6;
        if (j + 1 >= s.length() || s[j] != '\\' || s[j + 1] != 'u') {
          throw json::ParseException(
              "Expected low surrogate after high surrogate: " +
              s.substr(j - 6, 6));
        }

        // 0xDC00 <= low-surrogate code point <= 0xDFFF
        uint16_t low_surrogate_cp =
            to_code_point(s[j - 4], s[j - 3], s[j - 2], s[j - 1]);
        if (!(0xDC00 <= low_surrogate_cp && low_surrogate_cp <= 0xDFFF)) {
          throw json::ParseException(
              "Invalid low surrogate (" + s.substr(j - 6, 6) +
              ") after high surrogate: " + s.substr(j - 12, 6));
        }

        code_point = 0x10000 + ((high_surrogate_cp - 0xD800) << 10) +
                     (low_surrogate_cp - 0xDC00);
      }

      emit_utf8(code_point, res);
      i = j;
    }

    j++;
  }

  if (i < s.length()) {
    res.append(s, i, s.length() - i);
  }
}

}  // namespace

namespace json {

namespace syntax {

Parser::Parser(Lexer&& lexer) : lexer_(std::move(lexer)), root_(nullptr) {}

nodes::Node* Parser::parse() {
  ++lexer_;
  nodes::Node* json = parse_value();
  if (lexer_->type != TokenType::END_OF_JSON) {
    throw ParseException("Unexpected token: " + lexer_->value);
  }

  return json;
}

nodes::Node* Parser::parse_value() {
  switch (lexer_->type) {
    case TokenType::BOOLEAN:
      return parse_boolean();
    case TokenType::JSON_NULL:
      return parse_null();
    case TokenType::STRING:
      return parse_string();
    case TokenType::NUMBER:
      return parse_number();
    case TokenType::ARRAY_START:
      return parse_array();
    case TokenType::OBJECT_START:
      return parse_object();
    default:
      throw ParseException("Unexpected token: " + lexer_->value);
  }
}

nodes::Null* Parser::parse_null() {
  if (lexer_->type != TokenType::JSON_NULL) {
    throw ParseException("Unexpected token: " + lexer_->value);
  }

  ++lexer_;

  return new nodes::Null();
}

nodes::Boolean* Parser::parse_boolean() {
  if (lexer_->type != TokenType::BOOLEAN) {
    throw ParseException("Unexpected token: " + lexer_->value);
  }

  bool value = lexer_->value == "true";
  ++lexer_;

  return new nodes::Boolean(value);
}

nodes::String* Parser::parse_string() {
  if (lexer_->type != TokenType::STRING) {
    throw ParseException("Unexpected token: " + lexer_->value);
  }

  std::string value = "";
  resolve_unicode_sequences(lexer_->value, value);
  ++lexer_;

  return new nodes::String(value);
}

nodes::Number* Parser::parse_number() {
  if (lexer_->type != TokenType::NUMBER) {
    throw ParseException("Unexpected token: " + lexer_->value);
  }

  std::string_view value = lexer_->value;
  // integer
  size_t i = 0;
  if (value[i] == '-') {
    i++;
  }

  while (i < value.length() && isdigit(value[i])) {
    i++;
  }

  // fraction
  size_t j = i;
  if (j < value.length() && value[j] == '.') {
    j++;
    while (j < value.length() && isdigit(value[j])) {
      j++;
    }
  }

  // exponent
  size_t k = j;
  if (k < value.length() && tolower(value[k]) == 'e') {
    k++;
    if (value[k] == '+' || value[k] == '-') {
      k++;
    }

    while (k < value.length() && isdigit(value[k])) {
      k++;
    }
  }

  std::string_view integer = value.substr(0, i);

  std::string_view fraction;
  if (j > i + 1) {
    fraction = value.substr(i + 1, j - i - 1);
  }

  std::string_view exponent;
  if (k > j + 1) {
    exponent = value.substr(j + 1, k - j - 1);
  }

  nodes::Number* number =
      new nodes::Number(dsa::to_numeric(integer, fraction, exponent));
  ++lexer_;

  return number;
}

nodes::Array* Parser::parse_array() {
  if (lexer_->type != TokenType::ARRAY_START) {
    throw ParseException("Unexpected token: " + lexer_->value);
  }

  ++lexer_;

  nodes::Array* value = new nodes::Array();
  if (lexer_->type == TokenType::ARRAY_END) {
    ++lexer_;
    return value;
  }

  while (true) {
    value->push_back(parse_value());
    if (lexer_->type == TokenType::ARRAY_END) {
      break;
    }

    if (lexer_->type != TokenType::COMMA) {
      throw ParseException("Unexpected token: " + lexer_->value);
    }

    ++lexer_;
  }

  if (lexer_->type != TokenType::ARRAY_END) {
    throw ParseException("Unexpected token: " + lexer_->value);
  }

  ++lexer_;

  return value;
}

nodes::Object* Parser::parse_object() {
  if (lexer_->type != TokenType::OBJECT_START) {
    throw ParseException("Unexpected token: " + lexer_->value);
  }

  ++lexer_;

  nodes::Object* value = new nodes::Object();
  if (lexer_->type == TokenType::OBJECT_END) {
    ++lexer_;
    return value;
  }

  while (true) {
    nodes::String* key = parse_string();
    if (lexer_->type != TokenType::COLON) {
      throw ParseException("Unexpected token: " + lexer_->value);
    }

    ++lexer_;
    nodes::Node* val = parse_value();
    value->insert(key->get(), val);
    delete key;
    if (lexer_->type == TokenType::OBJECT_END) {
      break;
    }

    if (lexer_->type != TokenType::COMMA) {
      throw ParseException("Unexpected token: " + lexer_->value);
    }

    ++lexer_;
  }

  if (lexer_->type != TokenType::OBJECT_END) {
    throw ParseException("Unexpected token: " + lexer_->value);
  }

  ++lexer_;

  return value;
}

}  // namespace syntax

}  // namespace json
