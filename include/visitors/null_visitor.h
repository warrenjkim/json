#pragma once

#include <cstddef>
#include "nodes/node.h"
#include "visitor.h"

namespace json {

namespace visitors {

class NullVisitor : public Visitor {
 public:
  NullVisitor();

 public:
  void visit(Array& node) override;
  void visit(Boolean& node) override;
  void visit(Null& node) override;
  void visit(Number& node) override;
  void visit(Object& node) override;
  void visit(String& node) override;

 public:
  nullptr_t result();

 public:
  ~NullVisitor() = default;
};

}  // namespace visitors

}  // namespace json
