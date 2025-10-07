#pragma once

#include <string>

namespace warren {
namespace json {

enum class TokenType {
  OBJECT_START,
  OBJECT_END,
  ARRAY_START,
  ARRAY_END,
  QUOTE,
  COMMA,
  COLON,
  STRING,
  DOUBLE,
  INTEGRAL,
  BOOLEAN,
  JSON_NULL,
  UNKNOWN,
  END_OF_JSON
};

inline std::string to_string(TokenType type) {
  switch (type) {
    case TokenType::OBJECT_START:
      return "TokenType::ObjectStart";
    case TokenType::OBJECT_END:
      return "TokenType::ObjectEnd";
    case TokenType::ARRAY_START:
      return "TokenType::ArrayStart";
    case TokenType::ARRAY_END:
      return "TokenType::ArrayEnd";
    case TokenType::QUOTE:
      return "TokenType::Quote";
    case TokenType::COMMA:
      return "TokenType::Comma";
    case TokenType::COLON:
      return "TokenType::Colon";
    case TokenType::STRING:
      return "TokenType::String";
    case TokenType::DOUBLE:
      return "TokenType::Double";
    case TokenType::INTEGRAL:
      return "TokenType::Integral";
    case TokenType::BOOLEAN:
      return "TokenType::Boolean";
    case TokenType::JSON_NULL:
      return "TokenType::Null";
    case TokenType::UNKNOWN:
      return "TokenType::Unknown";
    case TokenType::END_OF_JSON:
      return "TokenType::EndOfJson";
  }

  __builtin_unreachable();
}

struct Token {
  TokenType type;
  std::string value;

  explicit Token(TokenType type, std::string value)
      : type(type), value(std::move(value)) {}

  explicit Token(TokenType type, char value)
      : type(type), value(std::string(1, value)) {}

  bool operator==(const Token& other) const noexcept {
    return value == other.value && type == other.type;
  }
};

inline std::string to_string(const Token& token) {
  return "(" + to_string(token.type) + ", " + token.value + ")";
}

}  // namespace json
}  // namespace warren
