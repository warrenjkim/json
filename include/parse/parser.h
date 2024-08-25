#pragma once

#include <cstdint>
#include <memory>
#include <queue>
#include <string>
#include <utility>

#include "nodes/array_node.h"
#include "nodes/node.h"
#include "nodes/number_node.h"
#include "nodes/object_node.h"
#include "nodes/string_node.h"

namespace json {

class JsonParser {
 public:
  static std::unique_ptr<Node> parse(std::queue<std::string>& tokens);
  static std::unique_ptr<Node> parse(const std::string_view json);

 private:
  static std::unique_ptr<ObjectNode> parse_object(
      std::queue<std::string>& tokens);
  static std::unique_ptr<ArrayNode> parse_array(
      std::queue<std::string>& tokens);
  static std::unique_ptr<StringNode> parse_string(
      std::queue<std::string>& tokens);
  static std::unique_ptr<NumberNode> parse_number(
      std::queue<std::string>& tokens);

 private:
  static std::pair<size_t, int64_t> parse_integer(const std::string_view token,
                                                  size_t& index);
  static std::pair<size_t, int64_t> parse_fraction(const std::string_view token,
                                                   size_t& index);
  static std::pair<size_t, int64_t> parse_exponent(const std::string_view token,
                                                   size_t& index);

 private:
  static std::string next(std::queue<std::string>& tokens);
};

}  // namespace json
