#pragma once

#include <cstddef>

#include "warren/internal/nodes/node.h"
#include "warren/internal/visitors/visitor.h"

namespace json {

namespace visitors {

class NullVisitor : public Visitor {
 public:
  NullVisitor();

 public:
  void visit(nodes::Array& node) override;
  void visit(nodes::Boolean& node) override;
  void visit(nodes::Null& node) override;
  void visit(nodes::Number& node) override;
  void visit(nodes::Object& node) override;
  void visit(nodes::String& node) override;

 public:
  nullptr_t result();

 public:
  ~NullVisitor() = default;
};

}  // namespace visitors

}  // namespace json
