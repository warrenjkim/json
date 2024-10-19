#pragma once

#include <optional>
#include <string_view>

#include "types/array.h"
#include "types/boolean.h"
#include "types/null.h"
#include "types/number.h"
#include "types/object.h"
#include "types/string.h"
#include "types/type.h"
#include "utils/queue.h"

namespace json {

class Token;

class Parser {
 public:
  static Type* parse(const std::string_view json);
  static Type* parse(utils::Queue<Token>& tokens);

 private:
  static Object* parse_object(utils::Queue<Token>& tokens,
                              const size_t indent_level = 0);
  static Array* parse_array(utils::Queue<Token>& tokens,
                            const size_t indent_level = 0);

 private:
  static Type* parse_value(utils::Queue<Token>& tokens,
                           const size_t indent_level = 0);
  static String* parse_string(utils::Queue<Token>& tokens,
                              const size_t indent_level = 0);
  static Number* parse_number(utils::Queue<Token>& tokens,
                              const size_t indent_level = 0);
  static std::optional<utils::Pair<std::string, Type*>> parse_key_value(
      utils::Queue<Token>& tokens, const size_t indent_level = 0);
  static Boolean* parse_boolean(utils::Queue<Token>& tokens,
                                const size_t indent_level = 0);
  static Null* parse_null(utils::Queue<Token>& tokens,
                          const size_t indent_level = 0);

 private:
  static std::optional<Token> next(utils::Queue<Token>& tokens);
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
