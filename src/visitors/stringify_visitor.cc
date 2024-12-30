#include "visitors/stringify_visitor.h"

#include <string>

#include "nodes/array.h"
#include "nodes/boolean.h"
#include "nodes/null.h"
#include "nodes/number.h"
#include "nodes/object.h"
#include "nodes/string.h"

namespace json {

namespace visitors {

StringifyVisitor::StringifyVisitor() : result_("") {}

void StringifyVisitor::visit(Array& node) {
  if (node.empty()) {
    result_ += "[]";

    return;
  }

  result_ += "[ ";

  std::vector<Node*> arr = node.get();
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

void StringifyVisitor::visit(Boolean& node) {
  result_ += node.get() ? "true" : "false";
}

void StringifyVisitor::visit(Null& node) { result_ += "null"; }

void StringifyVisitor::visit(Number& node) {
  result_ += std::to_string(node.get());
}

void StringifyVisitor::visit(Object& node) {
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

void StringifyVisitor::visit(String& node) {
  result_ += "\"" + node.get() + "\"";
}

std::string StringifyVisitor::result() { return result_; }

}  // namespace visitors

}  // namespace json
