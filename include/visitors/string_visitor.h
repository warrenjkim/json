#pragma once

#include <string>

#include "nodes/node.h"
#include "visitor.h"

namespace json {

namespace visitors {

class StringVisitor : public Visitor {
 public:
  StringVisitor();

 public:
  void visit(nodes::Array& node) override;
  void visit(nodes::Boolean& node) override;
  void visit(nodes::Null& node) override;
  void visit(nodes::Number& node) override;
  void visit(nodes::Object& node) override;
  void visit(nodes::String& node) override;

 public:
  std::string& result();

 public:
  ~StringVisitor() = default;

 private:
  std::string* result_;
};

}  // namespace visitors

}  // namespace json
