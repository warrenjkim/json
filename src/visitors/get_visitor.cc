#include "visitors/get_visitor.h"

#include <optional>
#include <stdexcept>
#include <string>

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

GetVisitor::GetVisitor(Node* root, const utils::Queue<std::string>& keys)
    : ast_(root), keys_(keys) {}

GetVisitor::GetVisitor(const Node* root, const utils::Queue<std::string>& keys)
    : ast_(const_cast<Node*>(root)), keys_(keys) {}

Node* GetVisitor::visit(const Array& node) {
  std::optional<std::string> key = keys_.dequeue();
  if (!key) {
    return const_cast<Array*>(&node);
  }

  try {
    return node.get().at(std::stoul(*key));
  } catch (const std::invalid_argument& e) {
    return nullptr;
  } catch (const std::out_of_range& e) {
    return nullptr;
  }
}

Node* GetVisitor::visit(const Boolean& node) {
  return keys_.empty() ? const_cast<Boolean*>(&node) : nullptr;
}

Node* GetVisitor::visit(const Null& node) {
  return keys_.empty() ? const_cast<Null*>(&node) : nullptr;
}

Node* GetVisitor::visit(const Number& node) {
  return keys_.empty() ? const_cast<Number*>(&node) : nullptr;
}

Node* GetVisitor::visit(const Object& node) {
  if (keys_.empty()) {
    return const_cast<Object*>(&node);
  }

  std::optional<Node*> result = node.get().get(*keys_.dequeue());
  if (!result) {
    return nullptr;
  }

  return result.value()->accept(*this);
}

Node* GetVisitor::visit(const String& node) {
  return keys_.empty() ? const_cast<String*>(&node) : nullptr;
}

}  // namespace visitors

}  // namespace json
