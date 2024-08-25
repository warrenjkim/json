#include "nodes/key_value_node.h"

#include <utility>  // move

#include "nodes/node.h"
#include "visitors/visitor.h"

namespace json {

void KeyValueNode::accept(JsonVisitor& visitor) const { visitor.visit(*this); }

KeyValueNode::KeyValueNode(std::string key, Node* value)
    : key_(std::move(key)), value_(value) {}

const std::string& KeyValueNode::key() const { return key_; }

const Node* KeyValueNode::value() const { return value_; }

}  // namespace json
