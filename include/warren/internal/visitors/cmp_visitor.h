#pragma once

#include "warren/internal/nodes/node.h"
#include "warren/internal/visitors/visitor.h"

namespace json {

namespace visitors {

class CmpVisitor : public ConstVisitor {
 public:
  CmpVisitor(nodes::Node* lhs);
  CmpVisitor(const nodes::Node* lhs);

 public:
  void visit(const nodes::Array& rhs) override;
  void visit(const nodes::Boolean& rhs) override;
  void visit(const nodes::Null& rhs) override;
  void visit(const nodes::Number& rhs) override;
  void visit(const nodes::Object& rhs) override;
  void visit(const nodes::String& rhs) override;

 public:
  bool result() const;

 private:
  bool result_;
  nodes::Node* lhs_;
};

}  // namespace visitors

}  // namespace json
