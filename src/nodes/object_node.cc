#include "nodes/object_node.h"

#include <typeinfo>
#include <vector>

#include "nodes/key_value_node.h"
#include "visitors/visitor.h"

namespace json {

ObjectNode::~ObjectNode() {
  for (KeyValueNode* property : properties_) {
    delete property;
  }
}

void ObjectNode::accept(JsonVisitor& visitor) const { visitor.visit(*this); }

bool ObjectNode::operator==(const Node& other) const {
  return typeid(*this) == typeid(other) &&
         std::equal(properties_.begin(), properties_.end(),
                    static_cast<const ObjectNode&>(other).properties_.begin(),
                    [](const Node* a, const Node* b) { return *a == *b; });
}

void ObjectNode::add(KeyValueNode* property) {
  properties_.push_back(property);
}

const std::vector<KeyValueNode*>& ObjectNode::get() const {
  return properties_;
}

const size_t ObjectNode::size() const { return properties_.size(); }

const bool ObjectNode::empty() const { return properties_.empty(); }

}  // namespace json
