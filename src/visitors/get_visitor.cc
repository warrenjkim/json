#include "visitors/get_visitor.h"

#include <stdexcept>
#include <string>
#include <string_view>

#include "json/exception.h"
#include "nodes/array.h"
#include "nodes/boolean.h"
#include "nodes/null.h"
#include "nodes/number.h"
#include "nodes/object.h"
#include "nodes/string.h"

namespace json {

namespace visitors {

GetVisitor::GetVisitor(const std::string_view key) : key_(key) {}

void GetVisitor::visit(Array& node) {
  if (key_.empty()) {
    result_ = &node;
    return;
  }
  try {
    result_ = node.get().at(std::stoul(key_));
    key_ = "";
    result_->accept(*this);
  } catch (const std::invalid_argument& e) {
    throw e;
  } catch (const std::out_of_range& e) {
    throw e;
  }
}

void GetVisitor::visit(Boolean& node) {
  if (!key_.empty()) {
    throw BadAccessException(
        "Bad access. Cannot access a property of a boolean value.");
  }
  result_ = &node;
}

void GetVisitor::visit(Null& node) {
  if (!key_.empty()) {
    throw BadAccessException(
        "Bad access. Cannot access a property of a null value.");
  }
  result_ = &node;
}

void GetVisitor::visit(Number& node) {
  if (!key_.empty()) {
    throw BadAccessException(
        "Bad access. Cannot access a property of a number value.");
  }
  result_ = &node;
}

void GetVisitor::visit(Object& node) {
  if (key_.empty()) {
    result_ = &node;
    return;
  }
  std::optional<Node*> result = node.get().get(key_);
  if (!result) {
    throw NotFoundException("Key \"" + key_ + "\" not found.");
  }
  result_ = *result;
  key_ = "";
  result_->accept(*this);
}

void GetVisitor::visit(String& node) {
  if (!key_.empty()) {
    throw BadAccessException(
        "Bad access. Cannot access a property of a string value.");
  }
  result_ = &node;
}

Node* GetVisitor::result() { return result_; }

}  // namespace visitors

}  // namespace json
