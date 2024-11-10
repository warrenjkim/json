#pragma once

#include <string>

#include "nodes/node.h"
#include "utils/queue.h"
#include "visitor.h"

namespace json {

namespace visitors {

class GetVisitor : public Visitor {
 public:
  GetVisitor(utils::Queue<std::string>& keys);
  GetVisitor(const utils::Queue<std::string>& keys);

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
  utils::Queue<std::string> keys_;
  Node* result_;
};

}  // namespace visitors

}  // namespace json
