#include "warren/internal/visitors/null_visitor.h"

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
