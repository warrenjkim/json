#pragma once

#include <vector>

#include "node.h"

namespace json {

class ArrayNode : public Node {
 public:
  void accept(JsonVisitor& visitor) const override;

 public:
  void add(Node* element);

 public:
  const std::vector<Node*>& get() const;

 private:
  std::vector<Node*> array_;

 public:
  ArrayNode() = default;
  ~ArrayNode() = default;
};

}  // namespace json
