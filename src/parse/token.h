#pragma once

#include <string>

namespace warren {
namespace json {
namespace syntax {

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

struct Token {
  TokenType type;
  std::string value;

  explicit Token(std::string value = "", TokenType type = TokenType::UNKNOWN)
      : type(type), value(std::move(value)) {}

  bool operator==(const Token& other) const noexcept {
    return value == other.value && type == other.type;
  }
};

}  // namespace syntax
}  // namespace json
}  // namespace warren
