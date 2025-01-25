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
  throw BadCastException("Expected type Null, but was Array.");
}

void NullVisitor::visit(Boolean& node) {
  throw BadCastException("Expected type Null, but was Boolean.");
}

void NullVisitor::visit(Null& node) { return; }

void NullVisitor::visit(Number& node) {
  throw BadCastException("Expected type Null, but was Number.");
}

void NullVisitor::visit(Object& node) {
  throw BadCastException("Expected type Null, but was Object.");
}

void NullVisitor::visit(String& node) {
  throw BadCastException("Expected type Null, but was String.");
}

nullptr_t NullVisitor::result() { return nullptr; }

}  // namespace visitors

}  // namespace json
