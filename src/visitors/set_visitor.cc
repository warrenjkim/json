#include "warren/internal/visitors/set_visitor.h"

#include <string>

#include "warren/internal/nodes/array.h"
#include "warren/internal/nodes/boolean.h"
#include "warren/internal/nodes/node.h"
#include "warren/internal/nodes/null.h"
#include "warren/internal/nodes/number.h"
#include "warren/internal/nodes/object.h"
#include "warren/internal/nodes/string.h"
#include "warren/json/exception.h"

namespace json {

namespace visitors {

SetVisitor::SetVisitor(nodes::Node** target, nodes::Node* value,
                       const std::string& key)
    : target_(target), value_(value), key_(key) {}

void SetVisitor::visit(nodes::Array& parent) {
  if (*target_) {
    delete *target_;
  }

  *target_ = value_;
  parent.get()[std::stoi(key_)] = *target_;
}

void SetVisitor::visit(nodes::Boolean& parent) {
  throw UnexpectedParentException(
      "Expected a parent of type nodes::Array or nodes::Object, but was "
      "nodes::Boolean.");
}

void SetVisitor::visit(nodes::Null& parent) {
  throw UnexpectedParentException(
      "Expected a parent of type nodes::Array or nodes::Object, but was "
      "nodes::Null.");
}

void SetVisitor::visit(nodes::Number& parent) {
  throw UnexpectedParentException(
      "Expected a parent of type nodes::Array or nodes::Object, but was "
      "nodes::Number.");
}

void SetVisitor::visit(nodes::Object& parent) {
  if (*target_) {
    delete *target_;
  }

  *target_ = value_;
  parent.insert(key_, *target_);
}

void SetVisitor::visit(nodes::String& parent) {
  throw UnexpectedParentException(
      "Expected a parent of type nodes::Array or nodes::Object, but was "
      "nodes::String.");
}

}  // namespace visitors

}  // namespace json
