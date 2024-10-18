#pragma once

#include "nodes/node.h"
#include "visitors/visitor.h"

namespace json {

class CmpVisitor : public JsonVisitor {
 public:
  CmpVisitor(Node* root);
  CmpVisitor(const Node* root);
  void visit(const ArrayNode& node) override;
  void visit(const BooleanNode& node) override;
  void visit(const NullNode& node) override;
  void visit(const NumberNode& node) override;
  void visit(const ObjectNode& node) override;
  void visit(const StringNode& node) override;
  bool result() const;

 private:
  bool result_;
  Node* expected_;
};

}  // namespace json
