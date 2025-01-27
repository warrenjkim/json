#pragma once

#include "nodes/node.h"
#include "utils/map.h"
#include "visitor.h"

namespace json {

namespace visitors {

class ObjectVisitor : public Visitor {
 public:
  ObjectVisitor();

 public:
  void visit(Array& node) override;
  void visit(Boolean& node) override;
  void visit(Null& node) override;
  void visit(Number& node) override;
  void visit(Object& node) override;
  void visit(String& node) override;

 public:
  utils::Map<std::string, Node*>& result();

 public:
  ~ObjectVisitor() = default;

 private:
  utils::Map<std::string, Node*>* result_;
};

}  // namespace visitors

}  // namespace json
