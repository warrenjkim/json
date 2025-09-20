#pragma once

#include "warren/json/internal/parse/lexer.h"
#include "warren/json/utils/types.h"
#include "warren/json/value.h"

namespace json {
namespace syntax {

class Parser {
 public:
  Parser(Lexer lexer);
  ~Parser() = default;

  Parser() = delete;
  Parser(const Parser&) = delete;
  Parser& operator=(const Parser&) = delete;

  Value parse();

 private:
  Value parse_value();

  nullptr_t parse_null();
  bool parse_boolean();
  std::string parse_string();
  Value parse_number();
  array_t parse_array();
  object_t parse_object();

 private:
  Lexer lexer_;
};

}  // namespace syntax
}  // namespace json
