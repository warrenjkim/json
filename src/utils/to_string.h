#pragma once

#include <string>

#include "warren/json/internal/parse/lexer.h"
#include "warren/json/internal/parse/token.h"
#include "warren/json/value.h"

namespace warren {
namespace json {

struct PrintOptions {
  size_t tab_width = 2;
  bool trailing_commas = false;
  bool compact = false;
};

std::string to_string(TokenType type);

std::string to_string(const Token& token);

std::string to_string(const Lexer::Error& error);

std::string to_string(const Value& value, const PrintOptions& opts = {});

inline std::ostream& operator<<(std::ostream& os, const Value& v) {
  os << to_string(v);
  return os;
}

}  // namespace json
}  // namespace warren
