#include "visitors/get_visitor.h"

#include <optional>
#include <stdexcept>
#include <string>

#include "json/exception.h"
#include "nodes/array.h"
#include "nodes/boolean.h"
#include "nodes/node.h"
#include "nodes/null.h"
#include "nodes/number.h"
#include "nodes/object.h"
#include "nodes/string.h"
#include "utils/queue.h"

namespace json {

namespace visitors {

GetVisitor::GetVisitor(utils::Queue<std::string>& keys) : keys_(keys) {}

GetVisitor::GetVisitor(const utils::Queue<std::string>& keys)
    : keys_(const_cast<utils::Queue<std::string>&>(keys)) {}

void GetVisitor::visit(Array& node) {
  std::optional<std::string> key = keys_.dequeue();
  if (!key) {
    result_ = &node;
    return;
  }

  try {
    result_ = node.get().at(std::stoul(*key));
    result_->accept(*this);
  } catch (const std::invalid_argument& e) {
    throw e;
  } catch (const std::out_of_range& e) {
    throw e;
  }
}

void GetVisitor::visit(Boolean& node) {
  if (!keys_.empty()) {
    throw BadAccessException(
        "Bad access. Cannot access a property of a boolean value.");
  }

  result_ = &node;
}

void GetVisitor::visit(Null& node) {
  if (!keys_.empty()) {
    throw BadAccessException(
        "Bad access. Cannot access a property of a null value.");
  }

  result_ = &node;
}

void GetVisitor::visit(Number& node) {
  if (!keys_.empty()) {
    throw BadAccessException(
        "Bad access. Cannot access a property of a number value.");
  }

  result_ = &node;
}

void GetVisitor::visit(Object& node) {
  if (keys_.empty()) {
    result_ = &node;
    return;
  }

  std::string key = *keys_.dequeue();
  std::optional<Node*> result = node.get().get(key);
  if (!result) {
    throw NotFoundException("Key \"" + key + "\" not found.");
  }

  result_ = *result;
  result_->accept(*this);
}

void GetVisitor::visit(String& node) {
  if (!keys_.empty()) {
    throw BadAccessException(
        "Bad access. Cannot access a property of a string value.");
  }

  result_ = &node;
}

Node* GetVisitor::result() { return result_; }

}  // namespace visitors

}  // namespace json
