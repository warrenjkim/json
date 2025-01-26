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

// TODO(replace DEBUG() with a tracing module. This probably won't get done
// until the Value API + the rest of the TODOs are finished.)

// TODO(modify the parser to support the actual ECMA-404 requirements [e.g.
// technically you are allowed to have a Number as a valid json, given that is
// the only thing specified in the string])
// Note: This sould be done after the tokenizer migration.
// context: This is pretty similar (i.e. almost identical) to a primitive json
// parser I wrote in ~2 days for the CS130 project. Since I knew that we would
// only ever get either an array or object, I did not allow for a json string to
// be anything else.
namespace json {

struct Token;

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
  ~Parser() = delete;
  Parser(Parser&&) = delete;
  Parser(const Parser&) = delete;
  Parser& operator=(Parser&&) = delete;
  Parser& operator=(const Parser&) = delete;
};

}  // namespace json
