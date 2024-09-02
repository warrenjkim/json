#include "nodes/array_node.h"

#include <algorithm>
#include <typeinfo>
#include <vector>

#include "nodes/node.h"
#include "visitors/visitor.h"

namespace json {

void ArrayNode::accept(JsonVisitor& visitor) const { visitor.visit(*this); }

bool ArrayNode::operator==(const Node& other) const {
  return typeid(*this) == typeid(other) &&
         std::equal(array_.begin(), array_.end(),
                    static_cast<const ArrayNode&>(other).array_.begin(),
                    [](const Node* a, const Node* b) { return *a == *b; });
}

void ArrayNode::add(Node* element) { array_.push_back(element); }

const size_t ArrayNode::size() const { return array_.size(); }

const bool ArrayNode::empty() const { return array_.empty(); }

const std::vector<Node*>& ArrayNode::get() const { return array_; }

}  // namespace json
