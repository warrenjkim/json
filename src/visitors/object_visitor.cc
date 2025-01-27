#include "visitors/object_visitor.h"

#include "json/exception.h"
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

void ObjectVisitor::visit(Array& node) {
  throw BadCastException("Expected type Object, but was Array.");
}

void ObjectVisitor::visit(Boolean& node) {
  throw BadCastException("Expected type Object, but was Boolean.");
}

void ObjectVisitor::visit(Null& node) {
  throw BadCastException("Expected type Object, but was Null.");
}

void ObjectVisitor::visit(Number& node) {
  throw BadCastException("Expected type Object, but was Number.");
}

void ObjectVisitor::visit(Object& node) { result_ = &node.get(); }

void ObjectVisitor::visit(String& node) {
  throw BadCastException("Expected type Object, but was String.");
}

utils::Map<std::string, Node*>& ObjectVisitor::result() { return *result_; }

}  // namespace visitors

}  // namespace json
