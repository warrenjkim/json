#include "nodes/null_node.h"

#include "nodes/visitors/visitor.h"

namespace json {

void NullNode::accept(Visitor& visitor) const { visitor.visit(*this); }

}  // namespace json
