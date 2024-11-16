#include "visitors/null_visitor.h"

#include "json/exception.h"
#include "nodes/array.h"
#include "nodes/boolean.h"
#include "nodes/null.h"
#include "nodes/number.h"
#include "nodes/object.h"
#include "nodes/string.h"

namespace json {

namespace visitors {

NullVisitor::NullVisitor() {}

void NullVisitor::visit(Array& node) {
  throw BadCastException("Value is not null.");
}

void NullVisitor::visit(Boolean& node) {
  throw BadCastException("Value is not null.");
}

void NullVisitor::visit(Null& node) { return; }

void NullVisitor::visit(Number& node) {
  throw BadCastException("Value is not null.");
}

void NullVisitor::visit(Object& node) {
  throw BadCastException("Value is not null.");
}

void NullVisitor::visit(String& node) {
  throw BadCastException("Value is not null.");
}

nullptr_t NullVisitor::result() { return nullptr; }

}  // namespace visitors

}  // namespace json
