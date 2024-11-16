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
  throw BadCastException("Value is not a string.");
}

void StringVisitor::visit(Boolean& node) {
  throw BadCastException("Value is not a string.");
}

void StringVisitor::visit(Null& node) {
  throw BadCastException("Value is not a string.");
}

void StringVisitor::visit(Number& node) {
  throw BadCastException("Value is not a string.");
}

void StringVisitor::visit(Object& node) {
  throw BadCastException("Value is not a string.");
}

void StringVisitor::visit(String& node) { result_ = &node.get(); }

std::string& StringVisitor::result() { return *result_; }

}  // namespace visitors

}  // namespace json
