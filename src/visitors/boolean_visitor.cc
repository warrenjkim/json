#include "visitors/boolean_visitor.h"

#include "json/exception.h"
#include "nodes/array.h"
#include "nodes/boolean.h"
#include "nodes/null.h"
#include "nodes/number.h"
#include "nodes/object.h"
#include "nodes/string.h"

namespace json {

namespace visitors {

BooleanVisitor::BooleanVisitor() {}

void BooleanVisitor::visit(Array& node) {
  throw BadCastException("Value is not a boolean.");
}

void BooleanVisitor::visit(Boolean& node) { result_ = node.get(); }

void BooleanVisitor::visit(Null& node) {
  throw BadCastException("Value is not a boolean.");
}

void BooleanVisitor::visit(Number& node) {
  throw BadCastException("Value is not a boolean.");
}

void BooleanVisitor::visit(Object& node) {
  throw BadCastException("Value is not a boolean.");
}

void BooleanVisitor::visit(String& node) {
  throw BadCastException("Value is not a boolean.");
}

bool BooleanVisitor::result() { return result_; }

}  // namespace visitors

}  // namespace json
