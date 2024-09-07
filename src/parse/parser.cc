#include "parse/parser.h"

#include <cmath>
#include <cstddef>
#include <optional>
#include <queue>
#include <string>
#include <string_view>

#include "nodes/key_value_node.h"
#include "nodes/node.h"
#include "nodes/object_node.h"
#include "parse/token.h"
#include "parse/tokenizer.h"
#include "utils/macros.h"
#include "utils/typedefs.h"

namespace json {

Node* Parser::parse(const std::string_view json) {
  std::optional<std::queue<Token>> tokens = Tokenizer::tokenize(json);
  if (!tokens.has_value() || tokens->empty()) {
    return nullptr;
  }

  return parse(*tokens);
}

Node* Parser::parse(std::queue<Token>& tokens) {
  if (tokens.empty()) {
    return nullptr;
  }

  switch (tokens.front().type) {
    case TokenType::OBJECT_START:
      return parse_object(tokens);
    case TokenType::ARRAY_START:
      return parse_array(tokens);
    default:
      return nullptr;
  }
}

ObjectNode* Parser::parse_object(std::queue<Token>& tokens,
                                 const size_t indent_level) {
  if (!expect_next(tokens, Token('{', TokenType::OBJECT_START))) {
    return nullptr;
  }

  ObjectNode* object = new ObjectNode();
  if (tokens.front() == Token('}', TokenType::OBJECT_END)) {
    tokens.pop();
    return object;
  }

  while (true) {
    if (tokens.empty()) {
      delete object;
      return nullptr;
    }

    KeyValueNode* key_value = parse_key_value(tokens);
    if (!key_value) {
      delete object;
      return nullptr;
    }

    object->add(key_value);

    std::optional<Token> token = next(tokens);
    if (!token.has_value()) {
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

ArrayNode* Parser::parse_array(std::queue<Token>& tokens,
                               const size_t indent_level) {
  if (!expect_next(tokens, Token('[', TokenType::ARRAY_START))) {
    return nullptr;
  }

  ArrayNode* array = new ArrayNode();
  if (tokens.front() == Token(']', TokenType::ARRAY_END)) {
    tokens.pop();
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

    array->add(value);

    std::optional<Token> token = next(tokens);
    if (!token.has_value()) {
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

Node* Parser::parse_value(std::queue<Token>& tokens,
                          const size_t indent_level) {
  if (tokens.empty()) {
    return nullptr;
  }

  switch (tokens.front().type) {
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

StringNode* Parser::parse_string(std::queue<Token>& tokens,
                                 const size_t indent_level) {
  if (!expect_next(tokens, Token('"', TokenType::QUOTE))) {
    return nullptr;
  }

  std::optional<Token> token = next(tokens);
  if (!token.has_value() || token->type != TokenType::STRING) {
    return nullptr;
  }

  StringNode* string = new StringNode(token->value);

  if (!expect_next(tokens, Token('"', TokenType::QUOTE))) {
    delete string;
    return nullptr;
  }

  return string;
}

NumberNode* Parser::parse_number(std::queue<Token>& tokens,
                                 const size_t indent_level) {
  std::string number_string = next(tokens)->value;
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

    return new NumberNode(base * std::pow(10.0, exponent));
  } catch (const std::exception& e) {
    ERROR("parse_number", std::string("Failed to parse number: ") + e.what(),
          indent_level);
    return nullptr;
  }
}

KeyValueNode* Parser::parse_key_value(std::queue<Token>& tokens,
                                      const size_t indent_level) {
  StringNode* string = parse_string(tokens, indent_level);
  if (!string) {
    return nullptr;
  }

  std::string key = string->get();
  delete string;

  if (!expect_next(tokens, Token(':', TokenType::COLON))) {
    return nullptr;
  }

  Node* value = parse_value(tokens, indent_level + 1);
  if (!value) {
    return nullptr;
  }

  return new KeyValueNode(key, value);
}

BooleanNode* Parser::parse_boolean(std::queue<Token>& tokens,
                                   const size_t indent_level) {
  std::optional<Token> token = next(tokens);
  if (token->value != "true" && token->value != "false") {
    return nullptr;
  }

  return new BooleanNode(token->value == "true" ? true : false);
}

NullNode* Parser::parse_null(std::queue<Token>& tokens,
                             const size_t indent_level) {
  if (!expect_next(tokens, Token("null", TokenType::JSON_NULL))) {
    return nullptr;
  }

  return new NullNode();
}

std::optional<Token> Parser::next(std::queue<Token>& tokens) {
  if (tokens.empty()) {
    return std::nullopt;
  }

  Token token = tokens.front();
  tokens.pop();

  return token;
}

const bool Parser::expect_next(std::queue<Token>& tokens,
                               const Token& expected) {
  std::optional<Token> token = next(tokens);
  if (!token.has_value() || *token != expected) {
    return false;
  }

  return true;
}

}  // namespace json
