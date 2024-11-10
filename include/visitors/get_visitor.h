#pragma once

#include <string>
#include <string_view>

#include "nodes/node.h"
#include "visitor.h"

namespace json {

namespace visitors {

class GetVisitor : public Visitor {
 public:
  GetVisitor(const std::string_view keys);

 public:
  void visit(Array& node) override;
  void visit(Boolean& node) override;
  void visit(Null& node) override;
  void visit(Number& node) override;
  void visit(Object& node) override;
  void visit(String& node) override;

 public:
  Node* result();

 public:
  ~GetVisitor() = default;

 private:
  Node* ast_;
  std::string key_;
  Node* result_;
};

}  // namespace visitors

}  // namespace json
