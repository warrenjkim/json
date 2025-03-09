#include "warren/internal/visitors/array_visitor.h"

#include <vector>

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

ArrayVisitor::ArrayVisitor() {}

void ArrayVisitor::visit(nodes::Array& node) { result_ = &node.get(); }

void ArrayVisitor::visit(nodes::Boolean& node) {
  throw BadCastException("Expected type Array, but was Boolean.");
}

void ArrayVisitor::visit(nodes::Null& node) {
  throw BadCastException("Expected type Array, but was Null.");
}

void ArrayVisitor::visit(nodes::Number& node) {
  throw BadCastException("Expected type Array, but was Number.");
}

void ArrayVisitor::visit(nodes::Object& node) {
  throw BadCastException("Expected type Array, but was Object.");
}

void ArrayVisitor::visit(nodes::String& node) {
  throw BadCastException("Expected type Array, but was String.");
}

std::vector<nodes::Node*>& ArrayVisitor::result() { return *result_; }

}  // namespace visitors

}  // namespace json
