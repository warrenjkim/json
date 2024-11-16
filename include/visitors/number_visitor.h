#pragma once

#include "nodes/node.h"
#include "visitor.h"

namespace json {

namespace visitors {

class NumberVisitor : public Visitor {
 public:
  NumberVisitor();

 public:
  void visit(Array& node) override;
  void visit(Boolean& node) override;
  void visit(Null& node) override;
  void visit(Number& node) override;
  void visit(Object& node) override;
  void visit(String& node) override;

 public:
  double result();

 public:
  ~NumberVisitor() = default;

 private:
  double result_;
};

}  // namespace visitors

}  // namespace json
