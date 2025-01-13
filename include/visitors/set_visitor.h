#pragma once

#include "visitor.h"

namespace json {

class Value;

namespace visitors {

class SetVisitor : public Visitor {
 public:
  SetVisitor(Node* target);

 public:
  void visit(Array& value) override;
  void visit(Boolean& value) override;
  void visit(Null& value) override;
  void visit(Number& value) override;
  void visit(Object& value) override;
  void visit(String& value) override;

 private:
  Node* target_;
};

}  // namespace visitors

}  // namespace json
