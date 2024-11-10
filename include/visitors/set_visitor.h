#pragma once

#include "json/value.h"
#include "nodes/node.h"
#include "visitor.h"

namespace json {

namespace visitors {

class SetVisitor : public Visitor {
 public:
  SetVisitor(Value& value);

 public:
  void visit(Array& node) override;
  void visit(Boolean& node) override;
  void visit(Null& node) override;
  void visit(Number& node) override;
  void visit(Object& node) override;
  void visit(String& node) override;

 public:
  ~SetVisitor() = default;

 private:
  Value& value_;
};

}  // namespace visitors

}  // namespace json
