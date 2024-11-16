#include "visitors/object_visitor.h"

#include "json/exception.h"
#include "nodes/array.h"
#include "nodes/boolean.h"
#include "nodes/null.h"
#include "nodes/number.h"
#include "nodes/object.h"
#include "nodes/string.h"
#include "utils/rbt.h"

namespace json {

namespace visitors {

ObjectVisitor::ObjectVisitor() {}

void ObjectVisitor::visit(Array& node) {
  throw BadCastException("Value is not an object.");
}

void ObjectVisitor::visit(Boolean& node) {
  throw BadCastException("Value is not an object.");
}

void ObjectVisitor::visit(Null& node) {
  throw BadCastException("Value is not an object.");
}

void ObjectVisitor::visit(Number& node) {
  throw BadCastException("Value is not an object.");
}

void ObjectVisitor::visit(Object& node) { result_ = &node.get(); }

void ObjectVisitor::visit(String& node) {
  throw BadCastException("Value is not an object.");
}

utils::Map<std::string, Node*>& ObjectVisitor::result() { return *result_; }

}  // namespace visitors

}  // namespace json
