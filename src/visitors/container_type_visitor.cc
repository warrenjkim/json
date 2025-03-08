#include "visitors/container_type_visitor.h"

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

ContainerTypeVisitor::ContainerTypeVisitor(Value::ContainerType& type)
    : type_(type) {}

void ContainerTypeVisitor::visit(nodes::Array& node) {
  type_ = Value::ContainerType::ARRAY;
}

void ContainerTypeVisitor::visit(nodes::Boolean&) {
  throw NonIterableTypeException(
      "Expected type Array or Object, but was Boolean.");
}

void ContainerTypeVisitor::visit(nodes::Null&) {
  throw NonIterableTypeException(
      "Expected type Array or Object, but was Null.");
}

void ContainerTypeVisitor::visit(nodes::Number&) {
  throw NonIterableTypeException(
      "Expected type Array or Object, but was Number.");
}

void ContainerTypeVisitor::visit(nodes::Object& node) {
  type_ = Value::ContainerType::OBJECT;
}

void ContainerTypeVisitor::visit(nodes::String&) {
  throw NonIterableTypeException(
      "Expected type Array or Object, but was String.");
}

}  // namespace visitors

}  // namespace json
