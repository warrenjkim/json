#pragma once

#include "nodes/array.h"
#include "nodes/boolean.h"
#include "nodes/node.h"
#include "nodes/null.h"
#include "nodes/number.h"
#include "nodes/object.h"
#include "nodes/string.h"
#include "visitor.h"

namespace json {

namespace visitors {

class CmpVisitor : public ConstVisitor {
 public:
  CmpVisitor(Node* lhs);
  CmpVisitor(const Node* lhs);
  void visit(const Array& rhs) override;
  void visit(const Boolean& rhs) override;
  void visit(const Null& rhs) override;
  void visit(const Number& rhs) override;
  void visit(const Object& rhs) override;
  void visit(const String& rhs) override;
  bool result() const;

 private:
  bool result_;
  Node* lhs_;
};

}  // namespace visitors

}  // namespace json
