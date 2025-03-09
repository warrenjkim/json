#pragma once

#include <string>

#include "warren/internal/nodes/node.h"
#include "warren/internal/visitors/visitor.h"

namespace json {

namespace visitors {

class StringifyVisitor : public Visitor {
 public:
  StringifyVisitor();

 public:
  void visit(nodes::Array& node) override;
  void visit(nodes::Boolean& node) override;
  void visit(nodes::Null& node) override;
  void visit(nodes::Number& node) override;
  void visit(nodes::Object& node) override;
  void visit(nodes::String& node) override;

 public:
  std::string result();

 private:
  std::string result_;
};

}  // namespace visitors

}  // namespace json
