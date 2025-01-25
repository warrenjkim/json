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
  throw BadCastException("Expected type Boolean, but was Array.");
}

void BooleanVisitor::visit(Boolean& node) { result_ = node.get(); }

void BooleanVisitor::visit(Null& node) {
  throw BadCastException("Expected type Boolean, but was Null.");
}

void BooleanVisitor::visit(Number& node) {
  throw BadCastException("Expected type Boolean, but was Number.");
}

void BooleanVisitor::visit(Object& node) {
  throw BadCastException("Expected type Boolean, but was Object.");
}

void BooleanVisitor::visit(String& node) {
  throw BadCastException("Expected type Boolean, but was String.");
}

bool BooleanVisitor::result() { return result_; }

}  // namespace visitors

}  // namespace json
