#pragma once

#include <optional>
#include <string>

#include "token.h"

namespace json {

namespace syntax {

struct Token;

class Lexer {
 public:
  Lexer(const std::string& json);
  Lexer(Lexer&&) = default;
  Lexer& operator=(Lexer&&) = default;

 public:
  Lexer() = delete;
  Lexer(const Lexer&) = delete;
  Lexer& operator=(const Lexer&) = delete;

 public:
  Token next_token();

 public:
  Lexer& operator++();
  const Token& operator*() const;
  const Token* operator->() const;
  bool eof() const;

 private:
  Token lex_null();
  Token lex_true();
  Token lex_false();

 private:
  Token lex_string();
  std::optional<std::string> lex_ctrl();

 private:
  Token lex_number();
  Token lex_integer();
  Token lex_fraction();
  Token lex_exponent();

 private:
  void strip_whitespace();

 private:
  size_t pos_;
  std::string json_;
  Token curr_;
};

}  // namespace syntax

}  // namespace json
