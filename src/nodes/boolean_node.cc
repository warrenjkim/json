#include "nodes/boolean_node.h"

#include <typeinfo>

#include "visitors/visitor.h"

namespace json {

void BooleanNode::accept(JsonVisitor& visitor) const { visitor.visit(*this); }

bool BooleanNode::operator==(const Node& other) const {
  return typeid(*this) == typeid(other) &&
         value_ == static_cast<const BooleanNode&>(other).value_;
}

BooleanNode::BooleanNode(const bool value) : value_(value) {}

const bool BooleanNode::get() const { return value_; }

}  // namespace json
