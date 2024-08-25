#pragma once

#include <cstddef>
#include <vector>

#include "node.h"
#include "nodes/key_value_node.h"

namespace json {

class ObjectNode : public Node {
 public:
  void accept(JsonVisitor& visitor) const override;

 public:
  void add(KeyValueNode&& property);

 public:
  const std::vector<KeyValueNode>& get() const;

 private:
  std::vector<KeyValueNode> properties_;

 public:
  ObjectNode() = default;
  ~ObjectNode() = default;
};

}  // namespace json
