#include "nodes/array_node.h"

#include <vector>

#include "nodes/node.h"
#include "visitors/visitor.h"

namespace json {

void ArrayNode::accept(JsonVisitor& visitor) const { visitor.visit(*this); }

void ArrayNode::add(Node* element) { array_.push_back(element); }

const std::vector<Node*>& ArrayNode::get() const { return array_; }

}  // namespace json
