#pragma once

#include <vector>

#include "node.h"
#include "type.h"

namespace json {

class Array : public Node {
 public:
  void accept(Visitor& visitor) const override;
  Node* accept(ReturnVisitor& visitor) const override;

 public:
  void add(Node* element);
  const size_t size() const;
  const bool empty() const;

 public:
  std::vector<Node*>& get();
  const std::vector<Node*>& get() const;

 public:
  const Type type() const override;

 private:
  std::vector<Node*> array_;
};

}  // namespace json
