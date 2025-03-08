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
  void visit(nodes::Array& node) override;
  void visit(nodes::Boolean& node) override;
  void visit(nodes::Null& node) override;
  void visit(nodes::Number& node) override;
  void visit(nodes::Object& node) override;
  void visit(nodes::String& node) override;

 public:
  std::vector<nodes::Node*>& result();

 public:
  ~ArrayVisitor() = default;

 private:
  std::vector<nodes::Node*>* result_;
};

}  // namespace visitors

}  // namespace json
