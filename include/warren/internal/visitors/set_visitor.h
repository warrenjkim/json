#pragma once

#include <string>

#include "warren/internal/visitors/visitor.h"

namespace json {

class Value;

namespace visitors {

class SetVisitor : public Visitor {
 public:
  SetVisitor(nodes::Node** target, nodes::Node* value, const std::string& key);

 public:
  void visit(nodes::Array& parent) override;
  void visit(nodes::Boolean& parent) override;
  void visit(nodes::Null& parent) override;
  void visit(nodes::Number& parent) override;
  void visit(nodes::Object& parent) override;
  void visit(nodes::String& parent) override;

 private:
  nodes::Node** target_;
  nodes::Node* value_;
  std::string key_;
};

}  // namespace visitors

}  // namespace json
