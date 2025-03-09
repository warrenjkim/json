#include "visitors/string_visitor.h"

#include <string>

#include "utils/exception.h"
#include "nodes/array.h"
#include "nodes/boolean.h"
#include "nodes/null.h"
#include "nodes/number.h"
#include "nodes/object.h"
#include "nodes/string.h"

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
