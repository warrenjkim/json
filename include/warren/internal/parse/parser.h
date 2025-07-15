#pragma once

#include "warren/internal/nodes/array.h"
#include "warren/internal/nodes/boolean.h"
#include "warren/internal/nodes/node.h"
#include "warren/internal/nodes/null.h"
#include "warren/internal/nodes/number.h"
#include "warren/internal/nodes/object.h"
#include "warren/internal/nodes/string.h"
#include "warren/internal/parse/lexer.h"

namespace json {

namespace syntax {

class Parser {
 public:
  Parser(Lexer&& lexer);

 public:
  ~Parser() = default;
  Parser(Parser&&) = default;
  Parser(const Parser&) = delete;
  Parser& operator=(Parser&&) = delete;
  Parser& operator=(const Parser&) = delete;

 public:
  nodes::Node* parse();

 private:
  nodes::Node* parse_value();

 private:
  nodes::Null* parse_null();
  nodes::Boolean* parse_boolean();
  nodes::String* parse_string();
  nodes::Number* parse_number();
  nodes::Array* parse_array();
  nodes::Object* parse_object();

 private:
  Lexer lexer_;
  nodes::Node* root_;
};

}  // namespace syntax

}  // namespace json
