#include "visitors/get_visitor.h"

#include <optional>
#include <stdexcept>
#include <string>

#include "types/array.h"
#include "types/boolean.h"
#include "types/null.h"
#include "types/number.h"
#include "types/object.h"
#include "types/string.h"
#include "types/type.h"
#include "utils/queue.h"

namespace json {

namespace visitors {

GetVisitor::GetVisitor(Type* root, const utils::Queue<std::string>& keys)
    : ast_(root), keys_(keys) {}

GetVisitor::GetVisitor(const Type* root, const utils::Queue<std::string>& keys)
    : ast_(const_cast<Type*>(root)), keys_(keys) {}

Type* GetVisitor::visit(const Array& node) {
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

Type* GetVisitor::visit(const Boolean& node) {
  return keys_.empty() ? const_cast<Boolean*>(&node) : nullptr;
}

Type* GetVisitor::visit(const Null& node) {
  return keys_.empty() ? const_cast<Null*>(&node) : nullptr;
}

Type* GetVisitor::visit(const Number& node) {
  return keys_.empty() ? const_cast<Number*>(&node) : nullptr;
}

Type* GetVisitor::visit(const Object& node) {
  if (keys_.empty()) {
    return const_cast<Object*>(&node);
  }

  std::optional<Type*> result = node.get().get(*keys_.dequeue());
  if (!result) {
    return nullptr;
  }

  return result.value()->accept(*this);
}

Type* GetVisitor::visit(const String& node) {
  return keys_.empty() ? const_cast<String*>(&node) : nullptr;
}

}  // namespace visitors

}  // namespace json
