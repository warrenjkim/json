#include "warren/internal/visitors/number_visitor.h"

#include "warren/internal/nodes/array.h"
#include "warren/internal/nodes/boolean.h"
#include "warren/internal/nodes/node.h"
#include "warren/internal/nodes/null.h"
#include "warren/internal/nodes/number.h"
#include "warren/internal/nodes/object.h"
#include "warren/internal/nodes/string.h"
#include "warren/json/exception.h"

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

dsa::Numeric NumberVisitor::result() { return result_; }

}  // namespace visitors

}  // namespace json
