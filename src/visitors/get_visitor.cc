#include "visitors/get_visitor.h"

#include <optional>
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

GetVisitor::GetVisitor() {}

GetVisitor::GetVisitor(const std::string_view key) : key_(key) {}

GetVisitor::GetVisitor(const size_t index) : index_(index) {}

void GetVisitor::visit(Array& node) {
  if (key_) {
    throw BadAccessException("Cannot access a property of an array.");
  }

  if (!index_) {
    result_ = &node;
    return;
  }

  try {
    result_ = node.get().at(*index_);
    index_ = std::nullopt;
    result_->accept(*this);
  } catch (const std::out_of_range& e) {
    throw e;
  }
}

void GetVisitor::visit(Boolean& node) {
  if (key_) {
    throw BadAccessException("Cannot access a property of a boolean value.");
  }

  if (index_) {
    throw BadAccessException("Cannot access an index of a boolean value.");
  }

  result_ = &node;
}

void GetVisitor::visit(Null& node) {
  if (key_) {
    throw BadAccessException("Cannot access a property of a null value.");
  }

  if (index_) {
    throw BadAccessException("Cannot access an index of a null value.");
  }

  result_ = &node;
}

void GetVisitor::visit(Number& node) {
  if (key_) {
    throw BadAccessException("Cannot access a property of a number value.");
  }

  if (index_) {
    throw BadAccessException("Cannot access an index of a number value.");
  }

  result_ = &node;
}

void GetVisitor::visit(Object& node) {
  if (index_) {
    throw BadAccessException("Cannot access an index of a number value.");
  }

  if (!key_ || key_->empty()) {
    result_ = &node;
    return;
  }

  std::optional<Node*> result = node.get().at(*key_);
  if (!result) {
    result_ = new Null();
    return;
  }

  result_ = *result;
  key_ = std::nullopt;
  result_->accept(*this);
}

void GetVisitor::visit(String& node) {
  if (key_) {
    throw BadAccessException("Cannot access a property of a string value.");
  }

  if (index_) {
    throw BadAccessException("Cannot access an index of a string value.");
  }

  result_ = &node;
}

Node* GetVisitor::result() { return result_; }

}  // namespace visitors

}  // namespace json
