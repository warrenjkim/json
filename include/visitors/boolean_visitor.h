#pragma once

#include "nodes/node.h"
#include "visitor.h"

namespace json {

namespace visitors {

class BooleanVisitor : public Visitor {
 public:
  BooleanVisitor();

 public:
  void visit(Array& node) override;
  void visit(Boolean& node) override;
  void visit(Null& node) override;
  void visit(Number& node) override;
  void visit(Object& node) override;
  void visit(String& node) override;

 public:
  bool result();

 public:
  ~BooleanVisitor() = default;

 private:
  bool result_;
};

}  // namespace visitors

}  // namespace json
