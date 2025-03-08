#pragma once

#include "nodes/node.h"
#include "visitor.h"

namespace json {

namespace visitors {

class BooleanVisitor : public Visitor {
 public:
  BooleanVisitor();

 public:
  void visit(nodes::Array& node) override;
  void visit(nodes::Boolean& node) override;
  void visit(nodes::Null& node) override;
  void visit(nodes::Number& node) override;
  void visit(nodes::Object& node) override;
  void visit(nodes::String& node) override;

 public:
  bool result();

 public:
  ~BooleanVisitor() = default;

 private:
  bool result_;
};

}  // namespace visitors

}  // namespace json
