#include "visitors/set_visitor.h"

#include "nodes/array.h"
#include "nodes/boolean.h"
#include "nodes/node.h"
#include "nodes/null.h"
#include "nodes/number.h"
#include "nodes/object.h"
#include "nodes/string.h"

namespace json {

namespace visitors {

SetVisitor::SetVisitor(Value& value) : value_(value) {}

void SetVisitor::visit(Array& node) {
  value_.data_.array = node.ptr();
  value_.type_ = Value::Type::ARRAY;
}

void SetVisitor::visit(Boolean& node) {
  value_.data_.boolean = node.ptr();
  value_.type_ = Value::Type::BOOLEAN;
}

void SetVisitor::visit(Null& node) {
  value_.data_.null = &node;
  value_.type_ = Value::Type::JSON_NULL;
}

void SetVisitor::visit(Number& node) {
  value_.data_.number = node.ptr();
  value_.type_ = Value::Type::NUMBER;
}

void SetVisitor::visit(Object& node) {
  value_.data_.object = node.ptr();
  value_.type_ = Value::Type::OBJECT;
}

void SetVisitor::visit(String& node) {
  value_.data_.string = node.ptr();
  value_.type_ = Value::Type::STRING;
}

}  // namespace visitors

}  // namespace json
