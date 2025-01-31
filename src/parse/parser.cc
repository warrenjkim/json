#include "parse/parser.h"

#include <cmath>  // pow
#include <cstddef>
#include <exception>
#include <optional>
#include <string>
#include <string_view>

#include "nodes/node.h"
#include "nodes/object.h"
#include "parse/token.h"
#include "parse/tokenizer.h"
#include "utils/macros.h"
#include "utils/pair.h"
#include "utils/queue.h"

namespace json {

Node* Parser::parse(const std::string_view json) {
  std::optional<json::utils::Queue<Token>> tokens = Tokenizer::tokenize(json);
  if (!tokens || tokens->empty()) {
    return nullptr;
  }

  return parse(*tokens);
}

Node* Parser::parse(json::utils::Queue<Token>& tokens) {
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

Object* Parser::parse_object(json::utils::Queue<Token>& tokens,
                             const size_t indent_level) {
  if (!expect_next(tokens, Token('{', TokenType::OBJECT_START))) {
    return nullptr;
  }

  Object* object = new Object();
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

    object->add(key_value->first, key_value->second);

    std::optional<Token> token = tokens.dequeue();
    if (!token) {
      delete object;
      return nullptr;
    }

    switch (token->type) {
      case TokenType::OBJECT_END:
        if (token->value != "}") {
          return nullptr;
        }

        return object;
      case TokenType::COMMA:
        if (token->value != ",") {
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

Array* Parser::parse_array(json::utils::Queue<Token>& tokens,
                           const size_t indent_level) {
  if (!expect_next(tokens, Token('[', TokenType::ARRAY_START))) {
    return nullptr;
  }

  Array* array = new Array();
  if (tokens.front() == Token(']', TokenType::ARRAY_END)) {
    tokens.dequeue();
    return array;
  }

  while (true) {
    if (tokens.empty()) {
      delete array;
      return nullptr;
    }

    Node* value = parse_value(tokens);
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
          return nullptr;
        }

        return array;
      case TokenType::COMMA:
        if (token->value != ",") {
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

Node* Parser::parse_value(json::utils::Queue<Token>& tokens,
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

String* Parser::parse_string(json::utils::Queue<Token>& tokens,
                             const size_t indent_level) {
  if (!expect_next(tokens, Token('"', TokenType::QUOTE))) {
    return nullptr;
  }

  std::optional<Token> token = tokens.dequeue();
  if (!token || token->type != TokenType::STRING) {
    return nullptr;
  }

  String* string = new String(token->value);

  if (!expect_next(tokens, Token('"', TokenType::QUOTE))) {
    delete string;
    return nullptr;
  }

  return string;
}

Number* Parser::parse_number(json::utils::Queue<Token>& tokens,
                             const size_t indent_level) {
  std::string number_string = tokens.dequeue()->value;
  size_t exponent_index = number_string.find_first_of("eE");
  double base = 0.0;
  int exponent = 0;
  try {
    if (exponent_index != std::string::npos) {
      base = std::stod(number_string.substr(0, exponent_index));
      exponent = std::stoi(number_string.substr(exponent_index + 1));
    } else {
      base = std::stod(number_string);
    }

    return new Number(base * std::pow(10, exponent));
  } catch (const std::exception& e) {
    ERROR("parse_number", std::string("Failed to parse number: ") + e.what(),
          indent_level);
    return nullptr;
  }
}

std::optional<utils::Pair<std::string, Node*>> Parser::parse_key_value(
    json::utils::Queue<Token>& tokens, const size_t indent_level) {
  String* string = parse_string(tokens, indent_level);
  if (!string) {
    return std::nullopt;
  }

  std::string key = string->get();
  delete string;

  if (!expect_next(tokens, Token(':', TokenType::COLON))) {
    return std::nullopt;
  }

  Node* value = parse_value(tokens, indent_level + 1);
  if (!value) {
    return std::nullopt;
  }

  return utils::Pair(key, value);
}

Boolean* Parser::parse_boolean(json::utils::Queue<Token>& tokens,
                               const size_t indent_level) {
  std::optional<Token> token = tokens.dequeue();
  if (token->value != "true" && token->value != "false") {
    return nullptr;
  }

  return new Boolean(token->value == "true" ? true : false);
}

Null* Parser::parse_null(json::utils::Queue<Token>& tokens,
                         const size_t indent_level) {
  if (!expect_next(tokens, Token("null", TokenType::JSON_NULL))) {
    return nullptr;
  }

  return new Null();
}

const bool Parser::expect_next(json::utils::Queue<Token>& tokens,
                               const Token& expected) {
  std::optional<Token> token = tokens.dequeue();

  return token && (*token == expected);
}

}  // namespace json
