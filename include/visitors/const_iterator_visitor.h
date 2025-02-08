#pragma once

#include "json/value.h"
#include "nodes/node.h"
#include "visitor.h"

namespace json {

namespace visitors {

class ConstIteratorVisitor : public Visitor {
 public:
  enum class Operation { INIT, BEGIN, END, INCREMENT, DECREMENT, DESTROY };

 public:
  ConstIteratorVisitor(Value::ConstIterator& it, const Operation op);

 public:
  void visit(Array& node) override;
  void visit(Boolean& node) override;
  void visit(Null& node) override;
  void visit(Number& node) override;
  void visit(Object& node) override;
  void visit(String& node) override;

  ConstIteratorVisitor& init();

 public:
  ConstIteratorVisitor() = delete;
  ~ConstIteratorVisitor() = default;

 private:
  Value::ConstIterator& it_;
  Operation op_;
};

}  // namespace visitors

}  // namespace json
