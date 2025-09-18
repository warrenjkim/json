#pragma once

#include "warren/json/internal/ast/array.h"
#include "warren/json/internal/ast/boolean.h"
#include "warren/json/internal/ast/node.h"
#include "warren/json/internal/ast/null.h"
#include "warren/json/internal/ast/number.h"
#include "warren/json/internal/ast/object.h"
#include "warren/json/internal/ast/string.h"
#include "warren/json/internal/parse/lexer.h"

namespace json {
namespace syntax {

class Parser {
 public:
  Parser(Lexer lexer);
  ~Parser() = default;

  Parser() = delete;
  Parser(const Parser&) = delete;
  Parser& operator=(const Parser&) = delete;

  ast::Node* parse();

 private:
  ast::Node* parse_value();

  ast::Null* parse_null();
  ast::Boolean* parse_boolean();
  ast::String* parse_string();
  ast::Number* parse_number();
  ast::Array* parse_array();
  ast::Object* parse_object();

 private:
  Lexer lexer_;
};

}  // namespace syntax
}  // namespace json
