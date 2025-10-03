#include "lexer.h"

#include <cctype>    // isdigit, isspace, isxdigit, tolower
#include <optional>  // nullopt, optional

#include "warren/json/internal/parse/token.h"

namespace json {
namespace syntax {

Lexer::Lexer(const std::string& json) : pos_(0), json_(json), curr_() {}

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
  if (pos_ >= json_.length()) {
    return Token("", TokenType::END_OF_JSON);
  }

  switch (json_[pos_]) {
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
      pos_++;
      return Token("[", TokenType::ARRAY_START);
    case ']':
      pos_++;
      return Token("]", TokenType::ARRAY_END);
    case '{':
      pos_++;
      return Token("{", TokenType::OBJECT_START);
    case ':':
      pos_++;
      return Token(":", TokenType::COLON);
    case '}':
      pos_++;
      return Token("}", TokenType::OBJECT_END);
    case ',':
      pos_++;
      return Token(",", TokenType::COMMA);
    default:
      error_ = Error(TokenType::UNKNOWN, pos_,
                     "unknown token: " + json_.substr(pos_));
      return Token(std::string(1, json_[pos_++]), TokenType::UNKNOWN);
  }
}

Token Lexer::lex_literal(const std::string& literal, TokenType type) {
  size_t start = pos_;
  if (pos_ + literal.length() > json_.length()) {
    std::string token = json_.substr(start);
    error_ = Error(
        type, start,
        "incomplete literal: got '" + token + "', expected '" + literal + "'");

    return Token(token, TokenType::UNKNOWN);
  }

  for (char c : literal) {
    if (json_[pos_++] != c) {
      std::string token = json_.substr(start, pos_ - start);
      error_ = Error(type, start,
                     "unexpected literal: got '" + token + "', expected '" +
                         literal + "'");

      return Token(token, TokenType::UNKNOWN);
    }
  }

  return Token(literal, type);
}

Token Lexer::lex_string() {
  size_t start = pos_;
  if (++pos_ >= json_.length()) {
    error_ = Error(TokenType::QUOTE, pos_ - 1, "unterminated string");
    return Token("", TokenType::UNKNOWN);
  }

  std::string res = "";
  while (pos_ < json_.length()) {
    char c = json_[pos_];
    if (c == '"') {
      pos_++;
      return Token(std::move(res), TokenType::STRING);
    }

    if (c == '\\') {
      size_t start = pos_;
      std::optional<std::string> ctrl = lex_ctrl();
      if (!ctrl) {
        std::string token = res + json_.substr(start, pos_ - start);
        error_ = Error(TokenType::STRING, start,
                       "invalid control character: " + token);
        return Token(token, TokenType::UNKNOWN);
      }

      res += *ctrl;
      continue;
    }

    res += c;
    pos_++;
  }

  error_ = Error(TokenType::QUOTE, start, "unterminated string");
  return Token(std::move(res), TokenType::UNKNOWN);
}

std::optional<std::string> Lexer::lex_ctrl() {
  if (++pos_ >= json_.length()) {
    return std::nullopt;
  }

  switch (json_[pos_++]) {
    case 'u': {
      std::string hex_digits;
      hex_digits.reserve(4);
      for (size_t i = 0; i < 4; i++) {
        if (pos_ >= json_.length() || !isxdigit(json_[pos_])) {
          return std::nullopt;
        }

        hex_digits += json_[pos_++];
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
  size_t start = pos_;
  if (json_[pos_] == '-') {
    pos_++;
  }

  if (pos_ >= json_.length()) {
    std::string token = json_.substr(start, pos_ - start);
    error_ = Error(TokenType::INTEGRAL, start, "invalid integer: " + token);
    return Token(token, TokenType::UNKNOWN);
  }

  if (json_[pos_] < '0' || json_[pos_] > '9') {
    std::string token = json_.substr(start, pos_ - start);
    error_ = Error(TokenType::INTEGRAL, start, "invalid integer: " + token);
    return Token(token, TokenType::UNKNOWN);
  }

  if (json_[pos_] == '0' && ++pos_ < json_.length() && isdigit(json_[pos_])) {
    std::string token = json_.substr(start, pos_ - start + 1);
    error_ = Error(TokenType::INTEGRAL, start, "invalid integer: " + token);
    return Token(token, TokenType::UNKNOWN);
  } else if (json_[pos_] >= '1' && json_[pos_] <= '9') {
    while (++pos_ < json_.length() && isdigit(json_[pos_]));
  }

  return Token(json_.substr(start, pos_ - start), TokenType::INTEGRAL);
}

Token Lexer::lex_fraction() {
  size_t start = pos_;
  if (json_[pos_] != '.') {
    return Token("", TokenType::INTEGRAL);
  }

  if (++pos_ >= json_.length() || !isdigit(json_[pos_])) {
    std::string token = json_.substr(start, pos_ - start);
    error_ = Error(TokenType::DOUBLE, start, "invalid fraction: " + token);
    return Token(token, TokenType::UNKNOWN);
  }

  while (pos_ < json_.length() && isdigit(json_[pos_])) {
    pos_++;
  }

  return Token(json_.substr(start, pos_ - start), TokenType::DOUBLE);
}

Token Lexer::lex_exponent() {
  size_t start = pos_;
  if (pos_ >= json_.length() || tolower(json_[pos_]) != 'e') {
    return Token("", TokenType::INTEGRAL);
  }

  if (++pos_ < json_.length() && (json_[pos_] == '+' || json_[pos_] == '-')) {
    pos_++;
  }

  if (pos_ >= json_.length() || !isdigit(json_[pos_])) {
    std::string token = json_.substr(start, pos_ - start);
    error_ = Error(TokenType::INTEGRAL, start, "invalid exponent: " + token);
    return Token(token, TokenType::UNKNOWN);
  }

  while (pos_ < json_.length() && isdigit(json_[pos_])) {
    pos_++;
  }

  return Token(json_.substr(start, pos_ - start), TokenType::INTEGRAL);
}

void Lexer::strip_whitespace() {
  while (pos_ < json_.length() && isspace(json_[pos_])) {
    pos_++;
  }
}

}  // namespace syntax
}  // namespace json
