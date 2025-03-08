#pragma once

#include <optional>
#include <string_view>

#include "nodes/array.h"
#include "nodes/boolean.h"
#include "nodes/node.h"
#include "nodes/null.h"
#include "nodes/number.h"
#include "nodes/object.h"
#include "nodes/string.h"
#include "utils/queue.h"

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
