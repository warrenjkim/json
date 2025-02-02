#pragma once

#include "json/value.h"
#include "nodes/node.h"
#include "visitor.h"

namespace json {

namespace visitors {

class IteratorVisitor : public Visitor {
 public:
  enum class Operation { BEGIN, END, INCREMENT, DECREMENT };

 public:
  IteratorVisitor(Value** curr, Value::Iterator::ContainerIterator& it,
                  Operation op);

 public:
  void visit(Array& node) override;
  void visit(Boolean& node) override;
  void visit(Null& node) override;
  void visit(Number& node) override;
  void visit(Object& node) override;
  void visit(String& node) override;

  Value::Iterator result();

 public:
  ~IteratorVisitor() = default;

 private:
  Value** curr_ = nullptr;
  Value::Iterator::ContainerIterator it_;
  Operation op_;
};

}  // namespace visitors

}  // namespace json
