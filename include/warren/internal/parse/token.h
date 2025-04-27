#pragma once

#include <string>

namespace json {

// TODO(DELETE ONCE MIGRATED OUT OF OLD PARSING LOGIC)
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

}  // namespace parse

}  // namespace json
