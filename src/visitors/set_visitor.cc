#include "visitors/set_visitor.h"

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
