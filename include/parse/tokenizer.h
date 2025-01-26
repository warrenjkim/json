#pragma once

#include <optional>
#include <string>
#include <string_view>

// TODO(remove std::pair as a dependency)
#include <utility>  // pair

#include "utils/queue.h"

// TODO(replace DEBUG() with a tracing module. This probably won't get done
// until the Value API + the rest of the TODOs are finished.)

// TODO(create a grammar-based tokenizer instead of whatever this is)
// context: This is pretty similar (i.e. almost identical) to a primitive json
// parser I wrote in ~2 days for the CS130 project. The only difference is that
// I enforce a functional pattern and use std::optional. Other than that, the
// logic is almost identical.
namespace json {

struct Token;

class Tokenizer {
 public:
  static std::optional<utils::Queue<Token>> tokenize(std::string_view json);

 private:
  static std::optional<size_t> tokenize_object(const std::string_view json,
                                               const size_t index,
                                               utils::Queue<Token>& tokens,
                                               const size_t indent_level = 0);
  static std::optional<size_t> tokenize_array(const std::string_view json,
                                              const size_t index,
                                              utils::Queue<Token>& tokens,
                                              const size_t indent_level = 0);
  static std::optional<size_t> tokenize_value(std::string_view json,
                                              const size_t index,
                                              utils::Queue<Token>& tokens,
                                              const size_t indent_level = 0);

 private:
  static std::optional<size_t> tokenize_string(const std::string_view json,
                                               const size_t index,
                                               utils::Queue<Token>& tokens,
                                               const size_t indent_level = 0);
  static std::optional<size_t> tokenize_number(const std::string_view json,
                                               const size_t index,
                                               utils::Queue<Token>& tokens,
                                               const size_t indent_level = 0);
  static std::optional<size_t> tokenize_key_value(
      std::string_view json, const size_t index, utils::Queue<Token>& tokens,
      const size_t indent_level = 0);

 private:
  static std::pair<size_t, std::string> tokenize_integer(
      const std::string_view json, size_t index, const size_t indent_level = 0);
  static std::optional<size_t> tokenize_logical_value(
      const std::string_view json, size_t index, utils::Queue<Token>& tokens,
      const size_t indent_level = 0);
  static std::optional<std::pair<size_t, std::string>>
  tokenize_control_character(std::string_view json, size_t index,
                             utils::Queue<Token>& tokens,
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
