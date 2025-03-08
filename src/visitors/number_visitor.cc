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

void NumberVisitor::visit(nodes::Array& node) {
  throw BadCastException("Expected type Number, but was Array.");
}

void NumberVisitor::visit(nodes::Boolean& node) {
  throw BadCastException("Expected type Number, but was Boolean.");
}

void NumberVisitor::visit(nodes::Null& node) {
  throw BadCastException("Expected type Number, but was Null.");
}

void NumberVisitor::visit(nodes::Number& node) { result_ = node.get(); }

void NumberVisitor::visit(nodes::Object& node) {
  throw BadCastException("Expected type Number, but was Object.");
}

void NumberVisitor::visit(nodes::String& node) {
  throw BadCastException("Expected type Number, but was String.");
}

double NumberVisitor::result() { return result_; }

}  // namespace visitors

}  // namespace json
