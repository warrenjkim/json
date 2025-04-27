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

 public:
  Lexer() = delete;
  Lexer(Lexer&&) = delete;
  Lexer(const Lexer&) = delete;
  Lexer& operator=(Lexer&&) = delete;
  Lexer& operator=(const Lexer&) = delete;

 public:
  std::optional<Token> next_token();

 public:
  Lexer& operator++();
  const Token& operator*() const;
  bool eof() const;

 private:
  std::optional<Token> lex_null();
  std::optional<Token> lex_true();
  std::optional<Token> lex_false();

 private:
  std::optional<Token> lex_string();
  std::optional<std::string> lex_ctrl();

 private:
  std::optional<Token> lex_number();
  std::optional<std::string> lex_integer();
  std::optional<std::string> lex_fraction();
  std::optional<std::string> lex_exponent();

 private:
  void strip_whitespace();

 private:
  size_t pos_;
  std::string json_;
  Token curr_;
};

}  // namespace parse

}  // namespace json
