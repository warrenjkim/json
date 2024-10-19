#include "nodes/number_node.h"

#include "nodes/visitors/visitor.h"

namespace json {

void NumberNode::accept(Visitor& visitor) const { visitor.visit(*this); }

NumberNode::NumberNode(double value) : value_(value) {}

const double NumberNode::get() const { return value_; }

}  // namespace json
