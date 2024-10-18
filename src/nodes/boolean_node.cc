#include "nodes/boolean_node.h"

#include "visitors/visitor.h"

namespace json {

void BooleanNode::accept(JsonVisitor& visitor) const { visitor.visit(*this); }

BooleanNode::BooleanNode(const bool value) : value_(value) {}

const bool BooleanNode::get() const { return value_; }

}  // namespace json
