#pragma once

#include <string>

#include "visitor.h"

namespace json {

class Value;

namespace visitors {

class SetVisitor : public Visitor {
 public:
  SetVisitor(Node** target, Node* value, const std::string& key);

 public:
  void visit(Array& parent) override;
  void visit(Boolean& parent) override;
  void visit(Null& parent) override;
  void visit(Number& parent) override;
  void visit(Object& parent) override;
  void visit(String& parent) override;

 private:
  Node** target_;
  Node* value_;
  std::string key_;
};

}  // namespace visitors

}  // namespace json
