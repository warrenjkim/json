#pragma once

#include <optional>
#include <string>

#include "warren/json/internal/parse/token.h"

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

    operator std::string() {
      auto type = [this]() -> std::string {
        switch (expected) {
          case TokenType::OBJECT_START:
            return "TokenType::ObjectStart";
          case TokenType::OBJECT_END:
            return "TokenType::ObjectEnd";
          case TokenType::ARRAY_START:
            return "TokenType::ArrayStart";
          case TokenType::ARRAY_END:
            return "TokenType::ArrayEnd";
          case TokenType::QUOTE:
            return "TokenType::Quote";
          case TokenType::COMMA:
            return "TokenType::Comma";
          case TokenType::COLON:
            return "TokenType::Colon";
          case TokenType::STRING:
            return "TokenType::String";
          case TokenType::DOUBLE:
            return "TokenType::Double";
          case TokenType::INTEGRAL:
            return "TokenType::Integral";
          case TokenType::BOOLEAN:
            return "TokenType::Boolean";
          case TokenType::JSON_NULL:
            return "TokenType::Null";
          case TokenType::UNKNOWN:
            return "TokenType::Unknown";
          case TokenType::END_OF_JSON:
            return "TokenType::EndOfJson";
        }

        __builtin_unreachable();
      };

      return "Error at position " + std::to_string(pos) + ". Expected " +
             type() + ": " + message;
    }
  };

  explicit Lexer(const std::string& json);
  Lexer(Lexer&&) = default;
  Lexer& operator=(Lexer&&) = default;

  Lexer() = delete;
  Lexer(const Lexer&) = delete;
  Lexer& operator=(const Lexer&) = delete;

  Lexer& operator++();
  const Token& operator*() const;
  const Token* operator->() const;
  operator bool() const;

  Error error() const;

  bool eof() const;
  bool ok() const;

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

  size_t pos_;
  std::string json_;
  Token curr_;
  std::optional<Error> error_;
};

}  // namespace json
}  // namespace warren
