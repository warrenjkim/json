#include "warren/internal/parse/lexer.h"

#include <cctype>     // isdigit, isspace, isxdigit, tolower
#include <cstdint>    // uint32_t
#include <optional>   // nullopt, optional
#include <stdexcept>  // invalid_argument, out_of_range

#include "warren/internal/parse/token.h"

namespace {

// https://www.ietf.org/rfc/rfc3629.txt
std::optional<std::string> to_unicode(const std::string& hex_digits) {
  uint32_t code_point = 0;
  try {
    code_point = (uint32_t)std::stoul(hex_digits, nullptr, 16);
  } catch (const std::out_of_range&) {
    return std::nullopt;
  } catch (const std::invalid_argument&) {
    return std::nullopt;
  }

  std::string result = "";
  if (code_point < 0x80) {
    result += (char)(code_point);
  } else if (code_point < 0x800) {
    result += (char)(0xC0 | (code_point >> 6));
    result += (char)(0x80 | (code_point & 0x3F));
  } else if (code_point < 0x10000) {
    result += (char)(0xE0 | (code_point >> 12));
    result += (char)(0x80 | ((code_point >> 6) & 0x3F));
    result += (char)(0x80 | (code_point & 0x3F));
  } else {
    result += (char)(0xF0 | (code_point >> 18));
    result += (char)(0x80 | ((code_point >> 12) & 0x3F));
    result += (char)(0x80 | ((code_point >> 6) & 0x3F));
    result += (char)(0x80 | (code_point & 0x3F));
  }

  return result;
}

}  // namespace

namespace json {

namespace parse {

Lexer::Lexer(const std::string& json)
    : pos_(0), json_(json), curr_('\0', TokenType::UNKNOWN) {}

std::optional<Token> Lexer::next_token() {
  std::optional<Token> token = std::nullopt;
  strip_whitespace();
  if (pos_ >= json_.length()) {
    return std::nullopt;
  }

  switch (json_[pos_]) {
    case 'n':
      token = lex_null();
      break;
    case 't':
      token = lex_true();
      break;
    case 'f':
      token = lex_false();
      break;
    case '"':
      token = lex_string();
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
      token = lex_number();
      break;
    case '[':
      pos_++;
      token = Token("[", TokenType::ARRAY_START);
      break;
    case ']':
      pos_++;
      token = Token("]", TokenType::ARRAY_END);
      break;
    case '{':
      pos_++;
      token = Token("{", TokenType::OBJECT_START);
      break;
    case ':':
      pos_++;
      token = Token(":", TokenType::COLON);
      break;
    case '}':
      pos_++;
      token = Token("}", TokenType::OBJECT_END);
      break;
    case ',':
      pos_++;
      token = Token(",", TokenType::COMMA);
      break;
    default:
      token = Token(std::string(1, json_[pos_++]), TokenType::UNKNOWN);
      break;
  }

  if (token) {
    curr_ = *token;
  }

  return token;
}

Lexer& Lexer::operator++() {
  std::optional<Token> next = next_token();
  curr_ = (next ? *next : Token('\0', TokenType::END_OF_JSON));

  return *this;
}

const Token& Lexer::operator*() const { return curr_; }

bool Lexer::eof() const { return curr_.type == TokenType::END_OF_JSON; }

std::optional<Token> Lexer::lex_null() {
  if (pos_ + 3 < json_.length() && json_[pos_] == 'n' &&
      json_[pos_ + 1] == 'u' && json_[pos_ + 2] == 'l' &&
      json_[pos_ + 3] == 'l') {
    pos_ += 4;

    return Token("null", TokenType::JSON_NULL);
  }

  return std::nullopt;
}

std::optional<Token> Lexer::lex_true() {
  if (pos_ + 3 < json_.length() && json_[pos_] == 't' &&
      json_[pos_ + 1] == 'r' && json_[pos_ + 2] == 'u' &&
      json_[pos_ + 3] == 'e') {
    pos_ += 4;

    return Token("true", TokenType::BOOLEAN);
  }

  return std::nullopt;
}

std::optional<Token> Lexer::lex_false() {
  if (pos_ + 4 < json_.length() && json_[pos_] == 'f' &&
      json_[pos_ + 1] == 'a' && json_[pos_ + 2] == 'l' &&
      json_[pos_ + 3] == 's' && json_[pos_ + 4] == 'e') {
    pos_ += 5;

    return Token("false", TokenType::BOOLEAN);
  }

  return std::nullopt;
}

std::optional<Token> Lexer::lex_string() {
  if (++pos_ >= json_.length()) {
    return std::nullopt;
  }

  std::string res = "";
  while (pos_ < json_.length()) {
    char c = json_[pos_];
    if (c == '"') {
      pos_++;
      return Token(res, TokenType::STRING);
    } else if (c == '\\') {
      std::optional<std::string> ctrl = lex_ctrl();
      if (!ctrl) {
        return std::nullopt;
      }

      res += *ctrl;
    } else {
      res += c;
      pos_++;
    }
  }

  return std::nullopt;
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

      return to_unicode(hex_digits);
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

std::optional<Token> Lexer::lex_number() {
  std::optional<std::string> integer = lex_integer();
  if (!integer) {
    return std::nullopt;
  }

  std::string number = *integer;
  if (std::optional<std::string> fraction = lex_fraction()) {
    number += *fraction;
  }

  if (std::optional<std::string> exponent = lex_exponent()) {
    number += *exponent;
  }

  return Token(number, TokenType::NUMBER);
}

std::optional<std::string> Lexer::lex_integer() {
  size_t start = pos_;
  if (json_[pos_] == '-') {
    pos_++;
  }

  if (pos_ >= json_.length()) {
    return std::nullopt;
  }

  if (json_[pos_] == '0') {
    pos_++;
  } else if (json_[pos_] >= '1' && json_[pos_] <= '9') {
    pos_++;
    while (pos_ < json_.length() && isdigit(json_[pos_])) {
      pos_++;
    }
  } else {
    return std::nullopt;
  }

  return json_.substr(start, pos_ - start);
}

std::optional<std::string> Lexer::lex_fraction() {
  size_t start = pos_;
  if (json_[pos_] != '.' || ++pos_ >= json_.length() || !isdigit(json_[pos_])) {
    return std::nullopt;
  }

  while (pos_ < json_.length() && isdigit(json_[pos_])) {
    pos_++;
  }

  return json_.substr(start, pos_ - start);
}

std::optional<std::string> Lexer::lex_exponent() {
  size_t start = pos_;
  if (tolower(json_[pos_]) != 'e' || ++pos_ >= json_.length()) {
    return std::nullopt;
  }

  if (json_[pos_] == '+' || json_[pos_] == '-') {
    pos_++;
  }

  if (pos_ >= json_.length() || !isdigit(json_[pos_])) {
    return std::nullopt;
  }

  while (pos_ < json_.length() && isdigit(json_[pos_])) {
    pos_++;
  }

  return json_.substr(start, pos_ - start);
}

void Lexer::strip_whitespace() {
  while (pos_ < json_.length() && isspace(json_[pos_])) {
    pos_++;
  }
}

}  // namespace parse

}  // namespace json
