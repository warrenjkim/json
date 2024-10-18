#include "nodes/key_value_node.h"

#include <utility>  // move

#include "nodes/node.h"

namespace json {

void KeyValueNode::accept(JsonVisitor& visitor) const { return; }

KeyValueNode::KeyValueNode(std::string key, Node* value)
    : key_(std::move(key)), value_(value) {}

const std::string& KeyValueNode::key() const { return key_; }

Node* KeyValueNode::value() { return value_; }

}  // namespace json
