#pragma once

#include <vector>

#include "node.h"

namespace json {

class Array : public Node {
 public:
  void accept(visitors::Visitor& visitor) override;
  void accept(visitors::ConstVisitor& visitor) const override;

 public:
  Node* clone() override;

 public:
  Array() = default;
  Array(const std::vector<Node*>& array);
  ~Array();

  Array(Array&&) = delete;
  Array(const Array&) = delete;
  Array& operator=(Array&&) = delete;
  Array& operator=(const Array&) = delete;

 public:
  void push_back(Node* element);
  const size_t size() const;
  const bool empty() const;

 public:
  std::vector<Node*>& get();
  const std::vector<Node*>& get() const;

 private:
  std::vector<Node*> array_;
};

}  // namespace json
