#pragma once

#include <string>

#include "utils/typedefs.h"

namespace json {

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
