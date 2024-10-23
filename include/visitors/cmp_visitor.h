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

class CmpVisitor : public Visitor {
 public:
  CmpVisitor(Node* root);
  CmpVisitor(const Node* root);
  void visit(const Array& node) override;
  void visit(const Boolean& node) override;
  void visit(const Null& node) override;
  void visit(const Number& node) override;
  void visit(const Object& node) override;
  void visit(const String& node) override;
  bool result() const;

 private:
  bool result_;
  Node* expected_;
};

}  // namespace visitors

}  // namespace json
