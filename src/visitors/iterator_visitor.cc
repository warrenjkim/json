#include "visitors/iterator_visitor.h"

#include <iostream>
#include <string>

#include "json/exception.h"
#include "nodes/array.h"
#include "nodes/boolean.h"
#include "nodes/node.h"
#include "nodes/null.h"
#include "nodes/number.h"
#include "nodes/object.h"
#include "nodes/string.h"

namespace json {

namespace visitors {

IteratorVisitor::IteratorVisitor(Value** curr,
                                 Value::Iterator::ContainerIterator& it,
                                 const Operation op)
    : curr_(curr), it_(it), op_(op) {}

void IteratorVisitor::visit(Array& node) {
  switch (op_) {
    case Operation::BEGIN:
      *it_.array_it = node.get().begin();
      break;
    case Operation::END:
      *it_.array_it = node.get().end();
      break;
    case Operation::INCREMENT:
      if (*it_.array_it == node.get().end()) {
        throw std::out_of_range("Iterator cannot be dereferenced");
      }

      ++(*it_.array_it);
      break;
    case Operation::DECREMENT:
      if (*it_.array_it == node.get().begin()) {
        throw std::out_of_range("Iterator cannot be dereferenced");
      }

      --(*it_.array_it);
      break;
  }

  Value* parent = (*curr_)->parent_;
  if (*curr_) {
    (*curr_)->~Value();
  }

  new (*curr_)
      Value((*it_.array_it == node.get().end()) ? nullptr : *(*it_.array_it));
  (*curr_)->parent_ = parent;
}

void IteratorVisitor::visit(Boolean&) {
  throw NonIterableTypeException(
      "Expected type Array or Object, but was Boolean.");
}

void IteratorVisitor::visit(Null&) {
  throw NonIterableTypeException(
      "Expected type Array or Object, but was Null.");
}

void IteratorVisitor::visit(Number&) {
  throw NonIterableTypeException(
      "Expected type Array or Object, but was Number.");
}

void IteratorVisitor::visit(Object& node) {
  switch (op_) {
    case Operation::BEGIN:
      *it_.map_it = node.get().begin();
      break;
    case Operation::END:
      *it_.map_it = node.get().end();
      break;
    case Operation::INCREMENT:
      if (*it_.map_it == node.get().end()) {
        throw std::out_of_range("Iterator cannot be dereferenced");
      }

      ++(*it_.map_it);
      break;
    case Operation::DECREMENT:
      if (*it_.map_it == node.get().begin()) {
        throw std::out_of_range("Iterator cannot be dereferenced");
      }

      --(*it_.map_it);
      break;
  }

  Value* parent = (*curr_)->parent_;
  if (*curr_) {
    (*curr_)->~Value();
  }

  new (*curr_) Value((*it_.map_it == node.get().end()) ? nullptr
                                                       : (*it_.map_it)->second);
  (*curr_)->parent_ = parent;
}

void IteratorVisitor::visit(String&) {
  throw NonIterableTypeException(
      "Expected type Array or Object, but was String.");
}

Value::Iterator IteratorVisitor::result() {
  return Value::Iterator((*curr_)->node_, (*curr_)->parent_, it_);
}

}  // namespace visitors

}  // namespace json
