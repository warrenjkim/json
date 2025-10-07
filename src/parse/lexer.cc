#include "warren/json/internal/parse/lexer.h"

#include <cctype>    // isdigit, isspace, isxdigit, tolower
#include <optional>  // nullopt, optional

#include "warren/json/internal/parse/token.h"

namespace warren {
namespace json {

Lexer::Lexer(const std::string& json) : reader_(json), curr_() {}

Lexer& Lexer::operator++() {
  curr_ = next_token();

  return *this;
}

const Token& Lexer::operator*() const { return curr_; }

const Token* Lexer::operator->() const { return &curr_; }

Lexer::operator bool() const { return !eof() && ok(); }

Lexer::Error Lexer::error() const { return *error_; }

bool Lexer::eof() const { return curr_.type == TokenType::END_OF_JSON; }

bool Lexer::ok() const { return !error_; }

Token Lexer::next_token() {
  strip_whitespace();
  if (reader_.eof()) {
    return Token("", TokenType::END_OF_JSON);
  }

  switch (reader_.peek()) {
    case 'n':
      return lex_literal("null", TokenType::JSON_NULL);
    case 't':
      return lex_literal("true", TokenType::BOOLEAN);
    case 'f':
      return lex_literal("false", TokenType::BOOLEAN);
    case '"':
      return lex_string();
    case '-':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      return lex_number();
    case '[':
      return Token(reader_.get(), TokenType::ARRAY_START);
    case ']':
      return Token(reader_.get(), TokenType::ARRAY_END);
    case '{':
      return Token(reader_.get(), TokenType::OBJECT_START);
    case ':':
      return Token(reader_.get(), TokenType::COLON);
    case '}':
      return Token(reader_.get(), TokenType::OBJECT_END);
    case ',':
      return Token(reader_.get(), TokenType::COMMA);
    default:
      error_ = Error(TokenType::UNKNOWN, reader_.tell(),
                     "unknown token: " + reader_.substr(reader_.tell()));
      return Token(reader_.get(), TokenType::UNKNOWN);
  }
}

Token Lexer::lex_literal(const std::string& literal, TokenType type) {
  size_t start = reader_.tell();
  std::string res;
  for (char c : literal) {
    if (reader_.eof()) {
      error_ = Error(
          type, start,
          "incomplete literal: got '" + res + "', expected '" + literal + "'");
      return Token(res, TokenType::UNKNOWN);
    }

    if (reader_.peek() != c) {
      error_ = Error(
          type, start,
          "unexpected literal: got '" + res + "', expected '" + literal + "'");
      return Token(res, TokenType::UNKNOWN);
    }

    res += reader_.get();
  }

  return Token(res, type);
}

Token Lexer::lex_string() {
  size_t start = reader_.tell();
  if (!reader_.expect('"') || reader_.eof()) {
    error_ = Error(TokenType::QUOTE, start, "unterminated string");
    return Token("", TokenType::UNKNOWN);
  }

  std::string res;
  while (!reader_.eof()) {
    if (reader_.expect('"')) {
      return Token(std::move(res), TokenType::STRING);
    }

    if (reader_.peek() == '\\') {
      size_t start = reader_.tell();
      std::optional<std::string> ctrl = lex_ctrl();
      if (!ctrl) {
        std::string token = res + reader_.substr(start, reader_.tell() - start);
        error_ = Error(TokenType::STRING, start,
                       "invalid control character: " + token);
        return Token(token, TokenType::UNKNOWN);
      }

      res += *ctrl;
      continue;
    }

    res += reader_.get();
  }

  error_ = Error(TokenType::QUOTE, start, "unterminated string");
  return Token(std::move(res), TokenType::UNKNOWN);
}

std::optional<std::string> Lexer::lex_ctrl() {
  if (!reader_.expect('\\') || reader_.eof()) {
    return std::nullopt;
  }

  switch (reader_.get()) {
    case 'u': {
      std::string hex_digits;
      hex_digits.reserve(4);
      for (size_t i = 0; i < 4; i++) {
        if (reader_.eof() || !isxdigit(reader_.peek())) {
          return std::nullopt;
        }

        hex_digits += reader_.get();
      }

      return "\\u" + hex_digits;
    }
    case '"':
      return "\"";
    case '\\':
      return "\\";
    case '/':
      return "/";
    case 'b':
      return "\b";
    case 'f':
      return "\f";
    case 'n':
      return "\n";
    case 'r':
      return "\r";
    case 't':
      return "\t";
    default:
      return std::nullopt;
  }
}

Token Lexer::lex_number() {
  Token integer = lex_integer();
  if (integer.type == TokenType::UNKNOWN) {
    return integer;
  }

  Token fraction = lex_fraction();
  if (fraction.type == TokenType::UNKNOWN) {
    fraction.value = integer.value + fraction.value;
    return fraction;
  }

  Token exponent = lex_exponent();
  if (exponent.type == TokenType::UNKNOWN) {
    exponent.value = integer.value + fraction.value + exponent.value;
    return exponent;
  }

  return Token(integer.value + fraction.value + exponent.value, fraction.type);
}

Token Lexer::lex_integer() {
  size_t start = reader_.tell();
  std::string integer;
  if (reader_.peek() == '-') {
    integer += reader_.get();
  }

  if (reader_.eof()) {
    error_ = Error(TokenType::INTEGRAL, start, "invalid integer: " + integer);
    return Token(integer, TokenType::UNKNOWN);
  }

  if (reader_.peek() < '0' || reader_.peek() > '9') {
    error_ = Error(TokenType::INTEGRAL, start, "invalid integer: " + integer);
    return Token(integer, TokenType::UNKNOWN);
  }

  integer += reader_.get();
  if (integer.back() == '0') {
    bool invalid = false;
    while (!reader_.eof() && isdigit(reader_.peek())) {
      invalid = true;
      integer += reader_.get();
    }

    if (invalid) {
      error_ = Error(TokenType::INTEGRAL, start, "invalid integer: " + integer);
      return Token(integer, TokenType::UNKNOWN);
    }

    return Token(integer, TokenType::INTEGRAL);
  }

  while (!reader_.eof() && isdigit(reader_.peek())) {
    integer += reader_.get();
  }

  return Token(integer, TokenType::INTEGRAL);
}

Token Lexer::lex_fraction() {
  size_t start = reader_.tell();
  std::string fraction;
  if (reader_.peek() != '.') {
    return Token(fraction, TokenType::INTEGRAL);
  }

  fraction += reader_.get();
  if (reader_.eof() || !isdigit(reader_.peek())) {
    error_ = Error(TokenType::DOUBLE, start, "invalid fraction: " + fraction);
    return Token(fraction, TokenType::UNKNOWN);
  }

  while (!reader_.eof() && isdigit(reader_.peek())) {
    fraction += reader_.get();
  }

  return Token(fraction, TokenType::DOUBLE);
}

Token Lexer::lex_exponent() {
  size_t start = reader_.tell();
  std::string exponent;
  if (reader_.eof() || tolower(reader_.peek()) != 'e') {
    return Token(exponent, TokenType::INTEGRAL);
  }

  exponent += reader_.get();
  if (!reader_.eof() && (reader_.peek() == '+' || reader_.peek() == '-')) {
    exponent += reader_.get();
  }

  if (reader_.eof() || !isdigit(reader_.peek())) {
    error_ = Error(TokenType::INTEGRAL, start, "invalid exponent: " + exponent);
    return Token(exponent, TokenType::UNKNOWN);
  }

  while (!reader_.eof() && isdigit(reader_.peek())) {
    exponent += reader_.get();
  }

  return Token(exponent, TokenType::INTEGRAL);
}

void Lexer::strip_whitespace() {
  while (!reader_.eof() && isspace(reader_.peek())) {
    (void)reader_.get();
  }
}

}  // namespace json
}  // namespace warren
