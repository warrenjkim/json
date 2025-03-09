#include "warren/internal/visitors/container_type_visitor.h"

#include "warren/internal/nodes/array.h"
#include "warren/internal/nodes/boolean.h"
#include "warren/internal/nodes/node.h"
#include "warren/internal/nodes/null.h"
#include "warren/internal/nodes/number.h"
#include "warren/internal/nodes/object.h"
#include "warren/internal/nodes/string.h"
#include "warren/json/exception.h"
#include "warren/json/value.h"

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
