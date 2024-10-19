#include "nodes/string_node.h"

#include <string>

#include "nodes/visitors/visitor.h"

namespace json {

void StringNode::accept(Visitor& visitor) const { visitor.visit(*this); }

StringNode::StringNode(std::string value) : value_(std::move(value)) {}

const std::string& StringNode::get() const { return value_; }

}  // namespace json
