#pragma once

#include "json/value.h"
#include "nodes/node.h"
#include "visitor.h"

namespace json {

namespace visitors {

class ContainerTypeVisitor : public Visitor {
 public:
  ContainerTypeVisitor(Value::ContainerType& type);

 public:
  void visit(Array& node) override;
  void visit(Boolean& node) override;
  void visit(Null& node) override;
  void visit(Number& node) override;
  void visit(Object& node) override;
  void visit(String& node) override;

 public:
  ContainerTypeVisitor() = delete;
  ~ContainerTypeVisitor() = default;

 private:
  Value::ContainerType& type_;
};

}  // namespace visitors

}  // namespace json
