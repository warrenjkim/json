#include "visitors/set_visitor.h"

#include <string>

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

SetVisitor::SetVisitor(Node** target, Node* value, const std::string& key)
    : target_(target), value_(value), key_(key) {}

void SetVisitor::visit(Array& parent) {
  if (*target_) {
    delete *target_;
  }

  *target_ = value_;
  parent.get()[std::stoi(key_)] = *target_;
}

void SetVisitor::visit(Boolean& parent) {
  throw UnexpectedParentException(
      "Expected a parent of type Array or Object, but was Boolean.");
}

void SetVisitor::visit(Null& parent) {
  throw UnexpectedParentException(
      "Expected a parent of type Array or Object, but was Null.");
}

void SetVisitor::visit(Number& parent) {
  throw UnexpectedParentException(
      "Expected a parent of type Array or Object, but was Number.");
}

void SetVisitor::visit(Object& parent) {
  if (*target_) {
    delete *target_;
  }

  *target_ = value_;
  parent.put(key_, *target_);
}

void SetVisitor::visit(String& parent) {
  throw UnexpectedParentException(
      "Expected a parent of type Array or Object, but was String.");
}

}  // namespace visitors

}  // namespace json
