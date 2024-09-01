#pragma once

#include <optional>
#include <queue>

#include "nodes/array_node.h"
#include "nodes/boolean_node.h"
#include "nodes/key_value_node.h"
#include "nodes/node.h"
#include "nodes/null_node.h"
#include "nodes/number_node.h"
#include "nodes/object_node.h"
#include "nodes/string_node.h"

namespace json {

class Token;

class Parser {
 public:
  static Node* parse(const std::string_view json);
  static Node* parse(std::queue<Token>& tokens);

 private:
  static ObjectNode* parse_object(std::queue<Token>& tokens,
                                  const size_t indent_level = 0);
  static ArrayNode* parse_array(std::queue<Token>& tokens,
                                const size_t indent_level = 0);

 private:
  static Node* parse_value(std::queue<Token>& tokens,
                           const size_t indent_level = 0);
  static StringNode* parse_string(std::queue<Token>& tokens,
                                  const size_t indent_level = 0);
  static NumberNode* parse_number(std::queue<Token>& tokens,
                                  const size_t indent_level = 0);
  static KeyValueNode* parse_key_value(std::queue<Token>& tokens,
                                       const size_t indent_level = 0);
  static BooleanNode* parse_boolean(std::queue<Token>& tokens,
                                    const size_t indent_level = 0);
  static NullNode* parse_null(std::queue<Token>& tokens,
                              const size_t indent_level = 0);

 private:
  static std::optional<Token> next(std::queue<Token>& tokens);
  static const bool expect_next(std::queue<Token>& tokens,
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
