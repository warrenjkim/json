#include "nodes/key_value_node.h"

#include <typeinfo>
#include <utility>  // move

#include "nodes/node.h"
#include "nodes/string_node.h"
#include "visitors/visitor.h"

namespace json {

void KeyValueNode::accept(JsonVisitor& visitor) const { visitor.visit(*this); }

bool KeyValueNode::operator==(const Node& other) const {
  return typeid(*this) == typeid(other) &&
         key_ == static_cast<const KeyValueNode&>(other).key_ &&
         value_->operator==(*static_cast<const KeyValueNode&>(other).value_);
}

KeyValueNode::KeyValueNode(std::string key, Node* value)
    : key_(std::move(key)), value_(value) {}

const std::string& KeyValueNode::key() const { return key_; }

const Node* KeyValueNode::value() const { return value_; }

}  // namespace json
