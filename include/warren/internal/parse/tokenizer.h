#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <utility>  // pair

#include "warren/internal/dsa/queue.h"

namespace json {

struct Token;

class Tokenizer {
 public:
  static std::optional<dsa::Queue<Token>> tokenize(std::string_view json);

 private:
  static std::optional<size_t> tokenize_object(const std::string_view json,
                                               const size_t index,
                                               dsa::Queue<Token>& tokens,
                                               const size_t indent_level = 0);
  static std::optional<size_t> tokenize_array(const std::string_view json,
                                              const size_t index,
                                              dsa::Queue<Token>& tokens,
                                              const size_t indent_level = 0);
  static std::optional<size_t> tokenize_value(std::string_view json,
                                              const size_t index,
                                              dsa::Queue<Token>& tokens,
                                              const size_t indent_level = 0);

 private:
  static std::optional<size_t> tokenize_string(const std::string_view json,
                                               const size_t index,
                                               dsa::Queue<Token>& tokens,
                                               const size_t indent_level = 0);
  static std::optional<size_t> tokenize_number(const std::string_view json,
                                               const size_t index,
                                               dsa::Queue<Token>& tokens,
                                               const size_t indent_level = 0);
  static std::optional<size_t> tokenize_key_value(
      std::string_view json, const size_t index, dsa::Queue<Token>& tokens,
      const size_t indent_level = 0);

 private:
  static std::pair<size_t, std::string> tokenize_integer(
      const std::string_view json, size_t index, const size_t indent_level = 0);
  static std::optional<size_t> tokenize_logical_value(
      const std::string_view json, size_t index, dsa::Queue<Token>& tokens,
      const size_t indent_level = 0);
  static std::optional<std::pair<size_t, std::string>>
  tokenize_control_character(std::string_view json, size_t index,
                             dsa::Queue<Token>& tokens,
                             const size_t indent_level = 0);

 private:
  static size_t strip_whitespace(std::string_view json, size_t index);
  static bool is_hex(const char c);

 private:
  Tokenizer() = delete;
  ~Tokenizer() = delete;
  Tokenizer(Tokenizer&&) = delete;
  Tokenizer(const Tokenizer&) = delete;
  Tokenizer& operator=(Tokenizer&&) = delete;
  Tokenizer& operator=(const Tokenizer&) = delete;
};

}  // namespace json
