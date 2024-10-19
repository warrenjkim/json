#include "nodes/object_node.h"

#include "nodes/visitors/visitor.h"
#include "utils/rbt.h"

namespace json {

ObjectNode::~ObjectNode() {}

void ObjectNode::accept(Visitor& visitor) const { visitor.visit(*this); }

void ObjectNode::add(const std::string& key, Node* value) {
  properties_[key] = value;
}

const utils::Map<std::string, Node*>& ObjectNode::get() const {
  return properties_;
}

const size_t ObjectNode::size() const { return properties_.size(); }

const bool ObjectNode::empty() const { return properties_.empty(); }

}  // namespace json
