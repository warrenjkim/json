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

class Token;

class Parser {
 public:
  static Node* parse(const std::string_view json);
  static Node* parse(utils::Queue<Token>& tokens);

 private:
  static Object* parse_object(utils::Queue<Token>& tokens,
                              const size_t indent_level = 0);
  static Array* parse_array(utils::Queue<Token>& tokens,
                            const size_t indent_level = 0);

 private:
  static Node* parse_value(utils::Queue<Token>& tokens,
                           const size_t indent_level = 0);
  static String* parse_string(utils::Queue<Token>& tokens,
                              const size_t indent_level = 0);
  static Number* parse_number(utils::Queue<Token>& tokens,
                              const size_t indent_level = 0);
  static std::optional<utils::Pair<std::string, Node*>> parse_key_value(
      utils::Queue<Token>& tokens, const size_t indent_level = 0);
  static Boolean* parse_boolean(utils::Queue<Token>& tokens,
                                const size_t indent_level = 0);
  static Null* parse_null(utils::Queue<Token>& tokens,
                          const size_t indent_level = 0);

 private:
  static const bool expect_next(utils::Queue<Token>& tokens,
                                const Token& expected);

 private:
  Parser() = delete;
  Parser(const Parser&) = delete;
  Parser(Parser&&) = delete;
  Parser& operator=(const Parser&) = delete;
  Parser& operator=(Parser&&) = delete;
  ~Parser() = delete;
};

}  // namespace json
