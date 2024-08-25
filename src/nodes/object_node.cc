#include "nodes/object_node.h"

#include <vector>

#include "nodes/key_value_node.h"
#include "visitors/visitor.h"

namespace json {

void ObjectNode::accept(JsonVisitor& visitor) const { visitor.visit(*this); }

void ObjectNode::add(KeyValueNode&& property) {
  properties_.push_back(std::move(property));
}

const std::vector<KeyValueNode>& ObjectNode::get() const { return properties_; }

}  // namespace json
