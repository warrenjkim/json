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
  throw BadCastException("Expected type Number, but was Array.");
}

void NumberVisitor::visit(Boolean& node) {
  throw BadCastException("Expected type Number, but was Boolean.");
}

void NumberVisitor::visit(Null& node) {
  throw BadCastException("Expected type Number, but was Null.");
}

void NumberVisitor::visit(Number& node) { result_ = node.get(); }

void NumberVisitor::visit(Object& node) {
  throw BadCastException("Expected type Number, but was Object.");
}

void NumberVisitor::visit(String& node) {
  throw BadCastException("Expected type Number, but was String.");
}

double NumberVisitor::result() { return result_; }

}  // namespace visitors

}  // namespace json
