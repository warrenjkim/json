#pragma once

#include <optional>
#include <string>

#include "warren/internal/dsa/queue.h"

namespace json {

struct Token;

class Lexer {
 public:
  Lexer() = delete;
  Lexer(const std::string& json);

 public:
  dsa::Queue<Token> lex();

 private:
  std::optional<Token> lex_null();
  std::optional<Token> lex_true();
  std::optional<Token> lex_false();
  std::optional<Token> lex_string();
  std::optional<std::string> lex_ctrl();
  std::optional<std::string> to_unicode(const std::string& hex_digits);
  std::optional<Token> lex_number();
  std::optional<std::string> lex_integer();
  std::optional<std::string> lex_fraction();
  std::optional<std::string> lex_exponent();

  void strip_whitespace();

 private:
  size_t pos_;
  std::string json_;
};

}  // namespace json
