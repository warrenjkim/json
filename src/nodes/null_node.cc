#include "nodes/null_node.h"

#include "visitors/visitor.h"

namespace json {

void NullNode::accept(JsonVisitor& visitor) const { visitor.visit(*this); }

}  // namespace json
