#include "parse/parser.h"

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <queue>
#include <string>

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
  std::optional<Token> token = next(tokens);
  if (!token.has_value()) {
    return nullptr;
  }

  switch (token->type) {
    case TokenType::OBJECT_START:
      return parse_object(tokens);
    case TokenType::OBJECT_END:
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
    std::optional<Token> token = next(tokens);
    if (!token.has_value()) {
      delete object;
      return nullptr;
    }

    KeyValueNode* key_value = parse_key_value(tokens);
    if (!key_value) {
      delete object;
      return nullptr;
    }

    object->add(key_value);

    token = next(tokens);
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
  if (!expect_next(tokens, Token('{', TokenType::OBJECT_START))) {
    return nullptr;
  }

  ArrayNode* array = new ArrayNode();
  if (tokens.front() == Token(']', TokenType::ARRAY_END)) {
    tokens.pop();
    return array;
  }

  while (true) {
    std::optional<Token> token = next(tokens);
    if (!token.has_value()) {
      delete array;
      return nullptr;
    }

    Node* value = parse_value(tokens);
    if (!value) {
      delete array;
      return nullptr;
    }

    array->add(value);

    token = next(tokens);
    if (!token.has_value()) {
      delete array;
      return nullptr;
    }

    switch (token->type) {
      case TokenType::ARRAY_END:
        if (token->value != "}") {
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
    case TokenType::STRING:
      return parse_string(tokens, indent_level + 1);
    case TokenType::OBJECT:
      return parse_object(tokens, indent_level + 1);
    case TokenType::ARRAY:
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
  std::optional<Token> token = next(tokens);
  if (!token.has_value() || token->type != TokenType::NUMBER) {
    return nullptr;
  }

  std::string number_string = token->value;
  size_t exponent_index = number_string.find('e');
  if (exponent_index == std::string::npos) {
    exponent_index = number_string.find('E');
  }

  int64_t exponent = 0;
  if (exponent_index != std::string::npos) {
    try {
      exponent = std::stoll((token->value).substr(exponent_index + 1));
      number_string = std::move(number_string.substr(0, exponent_index));
    } catch (const std::invalid_argument& e) {
      ERROR("parse_number", "", indent_level);

      return nullptr;
    } catch (const std::out_of_range& e) {
      ERROR("parse_number", "", indent_level);

      return nullptr;
    }
  }

  std::variant<int64_t, double> number;
  const int64_t kBase = 10;
  try {
    if (number_string.find('.') == std::string::npos) {
      int64_t value = std::stoll(number_string);
      int64_t exp = static_cast<int64_t>(pow(kBase, exponent));

      if (value > std::numeric_limits<int64_t>::max() / exp) {
        ERROR("parse_number", "Value exceeds int64_t limits", indent_level);

        return nullptr;
      }

      number = value * exp;
    } else {
      double value = std::stod(number_string);
      double exp = pow(kBase, exponent);

      if (value > std::numeric_limits<double>::max() / exp) {
        ERROR("parse_number", "Value exceeds double limits", indent_level);

        return nullptr;
      }

      number = value * exp;
    }
  } catch (const std::invalid_argument& e) {
    ERROR("parse_number", "", indent_level);

    return nullptr;
  } catch (const std::out_of_range& e) {
    ERROR("parse_number", "", indent_level);

    return nullptr;
  }

  return new NumberNode(number);
}

KeyValueNode* Parser::parse_key_value(std::queue<Token>& tokens,
                                      const size_t indent_level) {
  if (!expect_next(tokens, Token('"', TokenType::QUOTE))) {
    return nullptr;
  }

  std::optional<Token> token = next(tokens);
  if (!token.has_value() || token->type != TokenType::KEY) {
    return nullptr;
  }

  std::string key = token->value;

  if (!expect_next(tokens, Token('"', TokenType::QUOTE))) {
    return nullptr;
  }

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
  if (!token.has_value() || token->type != TokenType::BOOLEAN) {
    return nullptr;
  }

  if (token->value != "true" || token->value != "false") {
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
