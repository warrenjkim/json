#pragma once

#include <string>

#include "warren/json/internal/parse/lexer.h"
#include "warren/json/internal/parse/parser.h"
#include "warren/json/value.h"

namespace json {

inline Value operator""_json(const char* json, size_t len) {
  return syntax::Parser(syntax::Lexer(std::string(json, len))).parse();
}

inline Value parse(std::string json) {
  return syntax::Parser(syntax::Lexer(std::move(json))).parse();
}

}  // namespace json
