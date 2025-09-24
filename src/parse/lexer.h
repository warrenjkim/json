#pragma once

#include <optional>
#include <string>

#include "warren/json/internal/parse/token.h"

namespace json {
namespace syntax {

class Lexer {
 public:
  struct Error {
    TokenType expected;
    size_t pos;
    std::string message;

    explicit Error(TokenType expected, size_t pos, std::string message)
        : expected(expected), pos(pos), message(std::move(message)) {}
  };

  explicit Lexer(const std::string& json);
  Lexer(Lexer&&) = default;
  Lexer& operator=(Lexer&&) = default;

  Lexer() = delete;
  Lexer(const Lexer&) = delete;
  Lexer& operator=(const Lexer&) = delete;

  Token next_token();

  Lexer& operator++();
  const Token& operator*() const;
  const Token* operator->() const;
  operator bool() const;

  bool eof() const;
  bool has_error() const;

 private:
  Token lex_literal(const std::string& literal, TokenType type);

  Token lex_string();
  std::optional<std::string> lex_ctrl();

  Token lex_number();
  Token lex_integer();
  Token lex_fraction();
  Token lex_exponent();

  void strip_whitespace();

  size_t pos_;
  std::string json_;
  Token curr_;
  std::optional<Error> error_;
};

}  // namespace syntax
}  // namespace json
