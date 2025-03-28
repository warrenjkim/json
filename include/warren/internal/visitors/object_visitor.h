#pragma once

#include <string>

#include "warren/internal/dsa/map.h"
#include "warren/internal/nodes/node.h"
#include "warren/internal/visitors/visitor.h"

namespace json {

namespace visitors {

class ObjectVisitor : public Visitor {
 public:
  ObjectVisitor();

 public:
  void visit(nodes::Array& node) override;
  void visit(nodes::Boolean& node) override;
  void visit(nodes::Null& node) override;
  void visit(nodes::Number& node) override;
  void visit(nodes::Object& node) override;
  void visit(nodes::String& node) override;

 public:
  dsa::Map<std::string, nodes::Node*>& result();

 public:
  ~ObjectVisitor() = default;

 private:
  dsa::Map<std::string, nodes::Node*>* result_;
};

}  // namespace visitors

}  // namespace json
