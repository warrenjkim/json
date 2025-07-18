#include "warren/internal/parse/lexer.h"

#include <cctype>    // isdigit, isspace, isxdigit, tolower
#include <optional>  // nullopt, optional

#include "warren/internal/parse/token.h"

namespace json {

namespace syntax {

Lexer::Lexer(const std::string& json) : pos_(0), json_(json), curr_() {}

Token Lexer::next_token() {
  strip_whitespace();
  if (pos_ >= json_.length()) {
    return Token("", TokenType::END_OF_JSON);
  }

  switch (json_[pos_]) {
    case 'n':
      return lex_null();
      break;
    case 't':
      return lex_true();
      break;
    case 'f':
      return lex_false();
      break;
    case '"':
      return lex_string();
      break;
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
      break;
    case '[':
      pos_++;
      return Token("[", TokenType::ARRAY_START);
      break;
    case ']':
      pos_++;
      return Token("]", TokenType::ARRAY_END);
      break;
    case '{':
      pos_++;
      return Token("{", TokenType::OBJECT_START);
      break;
    case ':':
      pos_++;
      return Token(":", TokenType::COLON);
      break;
    case '}':
      pos_++;
      return Token("}", TokenType::OBJECT_END);
      break;
    case ',':
      pos_++;
      return Token(",", TokenType::COMMA);
      break;
    default:
      return Token(std::string(1, json_[pos_++]), TokenType::UNKNOWN);
      break;
  }
}

Lexer& Lexer::operator++() {
  curr_ = next_token();

  return *this;
}

const Token& Lexer::operator*() const { return curr_; }

const Token* Lexer::operator->() const { return &curr_; }

bool Lexer::eof() const { return curr_.type == TokenType::END_OF_JSON; }

Token Lexer::lex_null() {
  size_t start = pos_;
  if (pos_ + 3 >= json_.length()) {
    return Token(json_.substr(start), TokenType::UNKNOWN);
  }

  if (json_[pos_++] != 'n' || json_[pos_++] != 'u' || json_[pos_++] != 'l' ||
      json_[pos_++] != 'l') {
    return Token(json_.substr(start, pos_ - start), TokenType::UNKNOWN);
  }

  return Token("null", TokenType::JSON_NULL);
}

Token Lexer::lex_true() {
  size_t start = pos_;
  if (pos_ + 3 >= json_.length()) {
    pos_ = json_.length();
    return Token(json_.substr(start), TokenType::UNKNOWN);
  }

  if (json_[pos_++] != 't' || json_[pos_++] != 'r' || json_[pos_++] != 'u' ||
      json_[pos_++] != 'e') {
    return Token(json_.substr(start, pos_ - start), TokenType::UNKNOWN);
  }

  return Token("true", TokenType::BOOLEAN);
}

Token Lexer::lex_false() {
  size_t start = pos_;
  if (pos_ + 4 >= json_.length()) {
    pos_ = json_.length();
    return Token(json_.substr(start), TokenType::UNKNOWN);
  }

  if (json_[pos_++] != 'f' || json_[pos_++] != 'a' || json_[pos_++] != 'l' ||
      json_[pos_++] != 's' || json_[pos_++] != 'e') {
    return Token(json_.substr(start, pos_ - start), TokenType::UNKNOWN);
  }

  return Token("false", TokenType::BOOLEAN);
}

Token Lexer::lex_string() {
  if (++pos_ >= json_.length()) {
    return Token("", TokenType::UNKNOWN);
  }

  std::string res = "";
  while (pos_ < json_.length()) {
    char c = json_[pos_];
    if (c == '"') {
      pos_++;
      return Token(res, TokenType::STRING);
    }

    if (c == '\\') {
      size_t start = pos_;
      std::optional<std::string> ctrl = lex_ctrl();
      if (!ctrl) {
        return Token(res + json_.substr(start, pos_ - start),
                     TokenType::UNKNOWN);
      }

      res += *ctrl;
    } else {
      res += c;
      pos_++;
    }
  }

  return Token(res, TokenType::UNKNOWN);
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
  size_t start = pos_;

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

  return Token(integer.value + fraction.value + exponent.value,
               TokenType::NUMBER);
}

Token Lexer::lex_integer() {
  size_t start = pos_;

  if (pos_ >= json_.length()) {
    return Token("", TokenType::UNKNOWN);
  }

  if (json_[pos_] == '-') {
    pos_++;
  }

  if (pos_ >= json_.length()) {
    return Token(json_.substr(start, pos_ - start), TokenType::UNKNOWN);
  }

  if (json_[pos_] == '0' && ++pos_ < json_.length() && isdigit(json_[pos_])) {
    return Token(json_.substr(start, pos_ - start + 1), TokenType::UNKNOWN);
  } else if (json_[pos_] >= '1' && json_[pos_] <= '9') {
    while (++pos_ < json_.length() && isdigit(json_[pos_]));
  } else {
    return Token(json_.substr(start, pos_ - start), TokenType::UNKNOWN);
  }

  return Token(json_.substr(start, pos_ - start), TokenType::NUMBER);
}

Token Lexer::lex_fraction() {
  size_t start = pos_;
  if (json_[pos_] != '.') {
    return Token("", TokenType::NUMBER);
  }

  if (++pos_ >= json_.length() || !isdigit(json_[pos_])) {
    return Token(json_.substr(start, pos_ - start), TokenType::UNKNOWN);
  }

  while (pos_ < json_.length() && isdigit(json_[pos_])) {
    pos_++;
  }

  return Token(json_.substr(start, pos_ - start), TokenType::NUMBER);
}

Token Lexer::lex_exponent() {
  size_t start = pos_;
  if (pos_ >= json_.length() || tolower(json_[pos_]) != 'e') {
    return Token("", TokenType::NUMBER);
  }

  if (++pos_ < json_.length() && (json_[pos_] == '+' || json_[pos_] == '-')) {
    pos_++;
  }

  if (pos_ >= json_.length() || !isdigit(json_[pos_])) {
    return Token(json_.substr(start, pos_ - start), TokenType::UNKNOWN);
  }

  while (pos_ < json_.length() && isdigit(json_[pos_])) {
    pos_++;
  }

  return Token(json_.substr(start, pos_ - start), TokenType::NUMBER);
}

void Lexer::strip_whitespace() {
  while (pos_ < json_.length() && isspace(json_[pos_])) {
    pos_++;
  }
}

}  // namespace syntax

}  // namespace json
