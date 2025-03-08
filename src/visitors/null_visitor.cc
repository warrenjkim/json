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

void NullVisitor::visit(nodes::Array& node) {
  throw BadCastException("Expected type Null, but was Array.");
}

void NullVisitor::visit(nodes::Boolean& node) {
  throw BadCastException("Expected type Null, but was Boolean.");
}

void NullVisitor::visit(nodes::Null& node) { return; }

void NullVisitor::visit(nodes::Number& node) {
  throw BadCastException("Expected type Null, but was Number.");
}

void NullVisitor::visit(nodes::Object& node) {
  throw BadCastException("Expected type Null, but was Object.");
}

void NullVisitor::visit(nodes::String& node) {
  throw BadCastException("Expected type Null, but was String.");
}

nullptr_t NullVisitor::result() { return nullptr; }

}  // namespace visitors

}  // namespace json
