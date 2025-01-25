#include "visitors/string_visitor.h"

#include <string>

#include "json/exception.h"
#include "nodes/array.h"
#include "nodes/boolean.h"
#include "nodes/null.h"
#include "nodes/number.h"
#include "nodes/object.h"
#include "nodes/string.h"

namespace json {

namespace visitors {

StringVisitor::StringVisitor() {}

void StringVisitor::visit(Array& node) {
  throw BadCastException("Expected type String, but was Array.");
}

void StringVisitor::visit(Boolean& node) {
  throw BadCastException("Expected type String, but was Boolean.");
}

void StringVisitor::visit(Null& node) {
  throw BadCastException("Expected type String, but was Null.");
}

void StringVisitor::visit(Number& node) {
  throw BadCastException("Expected type String, but was Number.");
}

void StringVisitor::visit(Object& node) {
  throw BadCastException("Expected type String, but was Object.");
}

void StringVisitor::visit(String& node) { result_ = &node.get(); }

std::string& StringVisitor::result() { return *result_; }

}  // namespace visitors

}  // namespace json
