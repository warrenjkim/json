#pragma once

#include "warren/internal/dsa/numeric.h"
#include "warren/internal/nodes/node.h"
#include "warren/internal/visitors/visitor.h"

namespace json {

namespace visitors {

class NumberVisitor : public Visitor {
 public:
  NumberVisitor();

 public:
  void visit(nodes::Array& node) override;
  void visit(nodes::Boolean& node) override;
  void visit(nodes::Null& node) override;
  void visit(nodes::Number& node) override;
  void visit(nodes::Object& node) override;
  void visit(nodes::String& node) override;

 public:
  dsa::Numeric result();

 public:
  ~NumberVisitor() = default;

 private:
  dsa::Numeric result_;
};

}  // namespace visitors

}  // namespace json
