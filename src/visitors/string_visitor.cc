#include "warren/internal/visitors/string_visitor.h"

#include <string>

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

StringVisitor::StringVisitor() {}

void StringVisitor::visit(nodes::Array& node) {
  throw BadCastException("Expected type String, but was Array.");
}

void StringVisitor::visit(nodes::Boolean& node) {
  throw BadCastException("Expected type String, but was Boolean.");
}

void StringVisitor::visit(nodes::Null& node) {
  throw BadCastException("Expected type String, but was Null.");
}

void StringVisitor::visit(nodes::Number& node) {
  throw BadCastException("Expected type String, but was Number.");
}

void StringVisitor::visit(nodes::Object& node) {
  throw BadCastException("Expected type String, but was Object.");
}

void StringVisitor::visit(nodes::String& node) { result_ = &node.get(); }

std::string& StringVisitor::result() { return *result_; }

}  // namespace visitors

}  // namespace json
