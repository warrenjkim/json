#pragma once

#include <cstddef>
#include <vector>

#include "node.h"
#include "nodes/key_value_node.h"

namespace json {

class JsonVisitor;

class ObjectNode : public Node {
 public:
  ~ObjectNode();

 public:
  void accept(JsonVisitor& visitor) const override;
  bool operator==(const Node& other) const override;

 public:
  void add(KeyValueNode* property);
  const std::vector<KeyValueNode*>& get() const;
  const size_t size() const;
  const bool empty() const;

 private:
  std::vector<KeyValueNode*> properties_;

 public:
  ObjectNode() = default;
};

}  // namespace json
