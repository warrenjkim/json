#pragma once

#include <optional>
#include <string_view>

#include "warren/internal/dsa/queue.h"

namespace json {

struct Token;

class Lexer {
 public:
  Lexer() = delete;
  Lexer(std::string_view json);

 public:
  dsa::Queue<Token> lex();

 private:
  std::optional<Token> lex_null();
  std::optional<Token> lex_true();
  std::optional<Token> lex_false();
  std::optional<Token> lex_string();
  std::optional<std::string> lex_ctrl();
  std::optional<std::string> to_unicode(const std::string& hex_digits);

  void strip_whitespace();

 private:
  size_t pos_;
  std::string_view json_;
};

}  // namespace json
