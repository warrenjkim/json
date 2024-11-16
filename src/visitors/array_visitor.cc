#include "visitors/array_visitor.h"

#include <vector>

#include "json/exception.h"
#include "nodes/array.h"
#include "nodes/boolean.h"
#include "nodes/null.h"
#include "nodes/number.h"
#include "nodes/object.h"
#include "nodes/string.h"

namespace json {

namespace visitors {

ArrayVisitor::ArrayVisitor() {}

void ArrayVisitor::visit(Array& node) { result_ = &node.get(); }

void ArrayVisitor::visit(Boolean& node) {
  throw BadCastException("Value is not an array.");
}

void ArrayVisitor::visit(Null& node) {
  throw BadCastException("Value is not an array.");
}

void ArrayVisitor::visit(Number& node) {
  throw BadCastException("Value is not an array.");
}

void ArrayVisitor::visit(Object& node) {
  throw BadCastException("Value is not an array.");
}

void ArrayVisitor::visit(String& node) {
  throw BadCastException("Value is not an array.");
}

std::vector<Node*>& ArrayVisitor::result() { return *result_; }

}  // namespace visitors

}  // namespace json
