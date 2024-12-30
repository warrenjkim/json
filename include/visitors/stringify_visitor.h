#pragma once

#include <string>

#include "nodes/node.h"
#include "visitor.h"

namespace json {

namespace visitors {

class StringifyVisitor : public Visitor {
 public:
  StringifyVisitor();

 public:
  void visit(Array& node) override;
  void visit(Boolean& node) override;
  void visit(Null& node) override;
  void visit(Number& node) override;
  void visit(Object& node) override;
  void visit(String& node) override;

 public:
  std::string result();

 private:
  std::string result_;
};

}  // namespace visitors

}  // namespace json
