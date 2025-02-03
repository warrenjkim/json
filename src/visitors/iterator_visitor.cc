#include "visitors/iterator_visitor.h"

#include <string>
#include <vector>

#include "json/exception.h"
#include "json/value.h"
#include "nodes/array.h"
#include "nodes/boolean.h"
#include "nodes/node.h"
#include "nodes/null.h"
#include "nodes/number.h"
#include "nodes/object.h"
#include "nodes/string.h"

namespace json {

namespace visitors {

IteratorVisitor::IteratorVisitor(Value::Iterator& it, const Operation op)
    : it_(it), op_(op) {}

void IteratorVisitor::visit(Array& node) {
  switch (op_) {
    case Operation::INIT:
      it_.it_.array_it = (std::vector<Node*>::iterator*)::operator new(
          sizeof(std::vector<Node*>::iterator));
      return;
    case Operation::BEGIN:
      *(it_.it_.array_it) = node.get().begin();
      break;
    case Operation::END:
      *(it_.it_.array_it) = node.get().end();
      break;
    case Operation::INCREMENT:
      if (!(it_.it_.array_it) || *(it_.it_.array_it) == node.get().end()) {
        throw std::out_of_range("Iterator cannot be dereferenced");
      }

      ++(*(it_.it_.array_it));
      break;
    case Operation::DECREMENT:
      if (!(it_.it_.array_it) || *(it_.it_.array_it) == node.get().begin()) {
        throw std::out_of_range("Iterator cannot be dereferenced");
      }

      --(*(it_.it_.array_it));
      break;
    case Operation::DESTROY:
      ::operator delete(it_.curr_);
      ::operator delete(it_.it_.array_it);
      return;
  }

  Value** curr = &(it_.curr_);
  if (*curr) {
    (*curr)->~Value();
  }

  new (*curr)
      Value((*(it_.it_.array_it) == node.get().end()) ? nullptr
                                                      : *(*(it_.it_.array_it)));
  (*curr)->parent_ = it_.value_;
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
    case Operation::INIT:
      it_.it_.map_it =
          (utils::Map<std::string, Node*>::Iterator*)::operator new(
              sizeof(utils::Map<std::string, Node*>::Iterator));
      return;
    case Operation::BEGIN:
      *(it_.it_.map_it) = node.get().begin();
      break;
    case Operation::END:
      *(it_.it_.map_it) = node.get().end();
      break;
    case Operation::INCREMENT:
      if (!(it_.it_.map_it) || *(it_.it_.map_it) == node.get().end()) {
        throw std::out_of_range("Iterator cannot be dereferenced");
      }

      ++(*(it_.it_.map_it));
      break;
    case Operation::DECREMENT:
      if (!(it_.it_.map_it) || *(it_.it_.map_it) == node.get().begin()) {
        throw std::out_of_range("Iterator cannot be dereferenced");
      }

      --(*(it_.it_.map_it));
      break;
    case Operation::DESTROY:
      ::operator delete(it_.curr_);
      ::operator delete(it_.it_.map_it);
      return;
  }

  Value** curr = &(it_.curr_);
  if (*curr) {
    (*curr)->~Value();
  }

  new (*curr) Value((*(it_.it_.map_it) == node.get().end())
                        ? nullptr
                        : (*(it_.it_.map_it))->second);
  if (*(it_.it_.map_it) != node.get().end()) {
    (*curr)->key_ = (*(it_.it_.map_it))->first;
  }

  (*curr)->parent_ = it_.value_;
}

void IteratorVisitor::visit(String&) {
  throw NonIterableTypeException(
      "Expected type Array or Object, but was String.");
}

IteratorVisitor& IteratorVisitor::init() {
  if (it_.curr_) {
    return *this;
  }

  it_.curr_ = (Value*)::operator new(sizeof(Value));
  new (it_.curr_) Value();
  Operation op = op_;
  op_ = Operation::INIT;
  it_.value_->node_->accept(*this);
  op_ = op;
  return *this;
}

}  // namespace visitors

}  // namespace json
