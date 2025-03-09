#include "visitors/object_visitor.h"

#include "utils/exception.h"
#include "nodes/array.h"
#include "nodes/boolean.h"
#include "nodes/null.h"
#include "nodes/number.h"
#include "nodes/object.h"
#include "nodes/string.h"
#include "utils/map.h"

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

utils::Map<std::string, nodes::Node*>& ObjectVisitor::result() {
  return *result_;
}

}  // namespace visitors

}  // namespace json
