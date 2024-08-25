#pragma once

#include <string>

#include "node.h"

namespace json {

class KeyValueNode : public Node {
 public:
  void accept(JsonVisitor& visitor) const override;

 public:
  KeyValueNode(std::string key, Node* value);

 public:
  const std::string& key() const;
  const Node* value() const;

 private:
  std::string key_;
  Node* value_;

 public:
  KeyValueNode() = delete;
  ~KeyValueNode() = default;
};

}  // namespace json
