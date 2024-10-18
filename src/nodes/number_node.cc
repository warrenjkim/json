#include "nodes/number_node.h"

#include "visitors/visitor.h"

namespace json {

void NumberNode::accept(JsonVisitor& visitor) const { visitor.visit(*this); }

NumberNode::NumberNode(double value) : value_(value) {}

const double NumberNode::get() const { return value_; }

}  // namespace json
