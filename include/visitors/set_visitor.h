#pragma once

#include <cstddef>  // size_t
#include <optional>

#include "nodes/node.h"
#include "visitor.h"

namespace json {

namespace visitors {

class SetVisitor : public Visitor {
 public:
  SetVisitor(Node* node, Node* value, std::optional<size_t> index);

 public:
  void visit(Array& node) override;
  void visit(Boolean& node) override;
  void visit(Null& node) override;
  void visit(Number& node) override;
  void visit(Object& node) override;
  void visit(String& node) override;

 private:
  Node* node_;
  Node* value_;
  std::optional<size_t> index_;
};

}  // namespace visitors

}  // namespace json
