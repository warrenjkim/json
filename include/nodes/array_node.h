#pragma once

#include <vector>

#include "node.h"

namespace json {

class JsonVisitor;

class ArrayNode : public Node {
 public:
  void accept(JsonVisitor& visitor) const override;
  bool operator==(const Node& other) const override;

 public:
  void add(Node* element);
  const size_t size() const;
  const bool empty() const;

 public:
  const std::vector<Node*>& get() const;

 private:
  std::vector<Node*> array_;

 public:
  ArrayNode() = default;
  ~ArrayNode() = default;
};

}  // namespace json
