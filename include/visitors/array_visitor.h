#pragma once

#include <vector>

#include "nodes/node.h"
#include "visitor.h"

namespace json {

namespace visitors {

class ArrayVisitor : public Visitor {
 public:
  ArrayVisitor();

 public:
  void visit(Array& node) override;
  void visit(Boolean& node) override;
  void visit(Null& node) override;
  void visit(Number& node) override;
  void visit(Object& node) override;
  void visit(String& node) override;

 public:
  std::vector<Node*>& result();

 public:
  ~ArrayVisitor() = default;

 private:
  std::vector<Node*>* result_;
};

}  // namespace visitors

}  // namespace json
