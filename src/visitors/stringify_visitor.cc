#include "warren/internal/visitors/stringify_visitor.h"

#include <cmath>  // floor
#include <string>

#include "warren/internal/dsa/numeric.h"
#include "warren/internal/nodes/array.h"
#include "warren/internal/nodes/boolean.h"
#include "warren/internal/nodes/node.h"
#include "warren/internal/nodes/null.h"
#include "warren/internal/nodes/number.h"
#include "warren/internal/nodes/object.h"
#include "warren/internal/nodes/string.h"

namespace json {

namespace visitors {

StringifyVisitor::StringifyVisitor() : result_("") {}

void StringifyVisitor::visit(nodes::Array& node) {
  if (node.empty()) {
    result_ += "[]";

    return;
  }

  result_ += "[ ";

  std::vector<nodes::Node*> arr = node.get();
  bool first = true;
  for (const auto& value : arr) {
    if (!first) {
      result_ += ", ";
    }

    first = false;
    value->accept(*this);
  }

  result_ += " ]";
}

void StringifyVisitor::visit(nodes::Boolean& node) {
  result_ += node.get() ? "true" : "false";
}

void StringifyVisitor::visit(nodes::Null& node) { result_ += "null"; }

void StringifyVisitor::visit(nodes::Number& node) {
  dsa::Numeric number = node.get();
  result_ += (number.type == dsa::Numeric::INTEGRAL)
                 ? std::to_string(static_cast<int64_t>(number.intgr))
                 : std::to_string(number.dbl);
}

void StringifyVisitor::visit(nodes::Object& node) {
  if (node.empty()) {
    result_ += "{}";
    return;
  }

  const auto& map = node.get();

  result_ += "{ ";

  bool first = true;
  for (const auto& [key, value] : map) {
    if (!first) {
      result_ += ", ";
    }

    first = false;
    result_ += "\"" + key + "\": ";
    value->accept(*this);
  }

  result_ += " }";
}

void StringifyVisitor::visit(nodes::String& node) {
  result_ += "\"" + node.get() + "\"";
}

std::string StringifyVisitor::result() { return result_; }

}  // namespace visitors

}  // namespace json
