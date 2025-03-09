#pragma once

#include <optional>
#include <string_view>

#include "warren/internal/dsa/queue.h"
#include "warren/internal/nodes/array.h"
#include "warren/internal/nodes/boolean.h"
#include "warren/internal/nodes/node.h"
#include "warren/internal/nodes/null.h"
#include "warren/internal/nodes/number.h"
#include "warren/internal/nodes/object.h"
#include "warren/internal/nodes/string.h"

namespace json {

struct Token;

class Parser {
 public:
  static nodes::Node* parse(const std::string_view json);
  static nodes::Node* parse(utils::Queue<Token>& tokens);

 private:
  static nodes::Object* parse_object(utils::Queue<Token>& tokens,
                                     const size_t indent_level = 0);
  static nodes::Array* parse_array(utils::Queue<Token>& tokens,
                                   const size_t indent_level = 0);

 private:
  static nodes::Node* parse_value(utils::Queue<Token>& tokens,
                                  const size_t indent_level = 0);
  static nodes::String* parse_string(utils::Queue<Token>& tokens,
                                     const size_t indent_level = 0);
  static nodes::Number* parse_number(utils::Queue<Token>& tokens,
                                     const size_t indent_level = 0);
  static std::optional<utils::Pair<std::string, nodes::Node*>> parse_key_value(
      utils::Queue<Token>& tokens, const size_t indent_level = 0);
  static nodes::Boolean* parse_boolean(utils::Queue<Token>& tokens,
                                       const size_t indent_level = 0);
  static nodes::Null* parse_null(utils::Queue<Token>& tokens,
                                 const size_t indent_level = 0);

 private:
  static const bool expect_next(utils::Queue<Token>& tokens,
                                const Token& expected);

 private:
  Parser() = delete;
  ~Parser() = delete;
  Parser(Parser&&) = delete;
  Parser(const Parser&) = delete;
  Parser& operator=(Parser&&) = delete;
  Parser& operator=(const Parser&) = delete;
};

}  // namespace json
