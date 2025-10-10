#pragma once

#include <optional>
#include <string>

#include "warren/json/internal/parse/reader.h"
#include "warren/json/internal/parse/token.h"
#include "warren/json/trace/null_tracer.h"
#include "warren/json/trace/tracer.h"

namespace warren {
namespace json {

class Lexer {
 public:
  struct Error {
    TokenType expected;
    size_t pos;
    std::string message;

    explicit Error(TokenType expected, size_t pos, std::string message)
        : expected(expected), pos(pos), message(std::move(message)) {}

    bool operator==(const Error& other) const noexcept {
      return expected == other.expected && pos == other.pos &&
             message == other.message;
    }
  };

  explicit Lexer(std::string json,
                 trace::Tracer* tracer = new trace::NullTracer());

  Lexer(Lexer&&) noexcept = default;
  Lexer& operator=(Lexer&&) noexcept = default;

  Lexer(const Lexer&) = delete;
  Lexer& operator=(const Lexer&) = delete;

  Lexer& operator++();
  const Token& operator*() const noexcept;
  const Token* operator->() const noexcept;
  operator bool() const noexcept;

  bool ok() const noexcept;
  Error error() const;

  bool eof() const noexcept;

 private:
  Token next_token();

  Token lex_literal(const std::string& literal, TokenType type);

  Token lex_string();
  std::optional<std::string> lex_ctrl();

  Token lex_number();
  Token lex_integer();
  Token lex_fraction();
  Token lex_exponent();

  void strip_whitespace();

  Reader reader_;
  trace::Tracer* tracer_;
  Token curr_;
  std::optional<Error> error_;
};

}  // namespace json
}  // namespace warren
