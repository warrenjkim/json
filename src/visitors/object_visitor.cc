#include "warren/internal/visitors/object_visitor.h"

#include <map>

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

ObjectVisitor::ObjectVisitor() {}

void ObjectVisitor::visit(nodes::Array& node) {
  throw BadCastException("Expected type Object, but was Array.");
}

void ObjectVisitor::visit(nodes::Boolean& node) {
  throw BadCastException("Expected type Object, but was Boolean.");
}

void ObjectVisitor::visit(nodes::Null& node) {
  throw BadCastException("Expected type Object, but was Null.");
}

void ObjectVisitor::visit(nodes::Number& node) {
  throw BadCastException("Expected type Object, but was Number.");
}

void ObjectVisitor::visit(nodes::Object& node) { result_ = &node.get(); }

void ObjectVisitor::visit(nodes::String& node) {
  throw BadCastException("Expected type Object, but was String.");
}

std::map<std::string, nodes::Node*>& ObjectVisitor::result() {
  return *result_;
}

}  // namespace visitors

}  // namespace json
