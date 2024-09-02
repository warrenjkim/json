#include "nodes/null_node.h"

#include <typeinfo>

#include "visitors/visitor.h"

namespace json {

void NullNode::accept(JsonVisitor& visitor) const { visitor.visit(*this); }

bool NullNode::operator==(const Node& other) const {
  return typeid(*this) == typeid(other);
}

}  // namespace json
