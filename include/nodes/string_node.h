#pragma once

#include <string>

#include "node.h"

namespace json {

class JsonVisitor;

class StringNode : public Node {
 public:
  void accept(JsonVisitor& visitor) const override;
  bool operator==(const Node& other) const override;

 public:
  StringNode(std::string value);

 public:
  const std::string& get() const;

 private:
  std::string value_;

 public:
  StringNode() = delete;
  ~StringNode() = default;
};

}  // namespace json
