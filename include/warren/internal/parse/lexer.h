#pragma once

#include <optional>
#include <string>

namespace json {

struct Token;

class Lexer {
 public:
  Lexer() = delete;
  Lexer(const std::string& json);

 public:
  std::optional<Token> next_token();

 private:
  std::optional<Token> lex_null();
  std::optional<Token> lex_true();
  std::optional<Token> lex_false();

 private:
  std::optional<Token> lex_string();
  std::optional<std::string> lex_ctrl();
  std::optional<std::string> to_unicode(const std::string& hex_digits);

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
};

}  // namespace json
