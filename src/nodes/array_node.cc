#include "nodes/array_node.h"

#include <vector>

#include "nodes/node.h"
#include "nodes/visitors/visitor.h"

namespace json {

void ArrayNode::accept(Visitor& visitor) const { visitor.visit(*this); }

void ArrayNode::add(Node* element) { array_.push_back(element); }

const size_t ArrayNode::size() const { return array_.size(); }

const bool ArrayNode::empty() const { return array_.empty(); }

const std::vector<Node*>& ArrayNode::get() const { return array_; }

}  // namespace json
