#pragma once

#include "json/value.h"
#include "nodes/node.h"
#include "visitor.h"

namespace json {

namespace visitors {

class IteratorVisitor : public Visitor {
 public:
  enum class Operation { INIT, BEGIN, END, INCREMENT, DECREMENT, DESTROY };

 public:
  IteratorVisitor(Value::Iterator& it, const Operation op);

 public:
  void visit(Array& node) override;
  void visit(Boolean& node) override;
  void visit(Null& node) override;
  void visit(Number& node) override;
  void visit(Object& node) override;
  void visit(String& node) override;

  IteratorVisitor& init();

 public:
  IteratorVisitor() = delete;
  ~IteratorVisitor() = default;

 private:
  Value::Iterator& it_;
  Operation op_;
};

}  // namespace visitors

}  // namespace json
