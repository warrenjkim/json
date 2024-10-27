#pragma once

#include <vector>

#include "node.h"

namespace json {

class Array : public Node {
 public:
  void accept(visitors::Visitor& visitor) const override;
  Node* accept(visitors::ReturnVisitor& visitor) const override;

 public:
  void add(Node* element);
  const size_t size() const;
  const bool empty() const;

 public:
  std::vector<Node*>& get();
  const std::vector<Node*>& get() const;

 private:
  std::vector<Node*> array_;
};

}  // namespace json
