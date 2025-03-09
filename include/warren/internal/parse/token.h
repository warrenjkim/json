#pragma once

#include <string>

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
  NUMBER,
  BOOLEAN,
  JSON_NULL,
  UNKNOWN,
  END_OF_JSON
};

struct Token {
  std::string value;
  TokenType type;

  Token(std::string value = "", TokenType type = TokenType::UNKNOWN)
      : type(type), value(std::move(value)) {}
  Token(char value = '\0', TokenType type = TokenType::UNKNOWN)
      : type(type), value(1, value) {}

  bool operator==(const Token& other) const {
    return value == other.value && type == other.type;
  }
};

}  // namespace json
