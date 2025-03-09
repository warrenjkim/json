#pragma once

#include "warren/internal/nodes/node.h"
#include "warren/json/value.h"
#include "warren/internal/visitors/visitor.h"

namespace json {

namespace visitors {

class ContainerTypeVisitor : public Visitor {
 public:
  ContainerTypeVisitor(Value::ContainerType& type);

 public:
  void visit(nodes::Array& node) override;
  void visit(nodes::Boolean& node) override;
  void visit(nodes::Null& node) override;
  void visit(nodes::Number& node) override;
  void visit(nodes::Object& node) override;
  void visit(nodes::String& node) override;

 public:
  ContainerTypeVisitor() = delete;
  ~ContainerTypeVisitor() = default;

 private:
  Value::ContainerType& type_;
};

}  // namespace visitors

}  // namespace json
