#include "visitors/number_visitor.h"

#include "json/exception.h"
#include "nodes/array.h"
#include "nodes/boolean.h"
#include "nodes/null.h"
#include "nodes/number.h"
#include "nodes/object.h"
#include "nodes/string.h"

namespace json {

namespace visitors {

NumberVisitor::NumberVisitor() {}

void NumberVisitor::visit(Array& node) {
  throw BadCastException("Value is not a number.");
}

void NumberVisitor::visit(Boolean& node) {
  throw BadCastException("Value is not a number.");
}

void NumberVisitor::visit(Null& node) {
  throw BadCastException("Value is not a number.");
}

void NumberVisitor::visit(Number& node) { result_ = node.get(); }

void NumberVisitor::visit(Object& node) {
  throw BadCastException("Value is not a number.");
}

void NumberVisitor::visit(String& node) {
  throw BadCastException("Value is not a number.");
}

double NumberVisitor::result() { return result_; }

}  // namespace visitors

}  // namespace json
