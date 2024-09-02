#include "nodes/string_node.h"

#include <string>
#include <typeinfo>

#include "visitors/visitor.h"

namespace json {

void StringNode::accept(JsonVisitor& visitor) const { visitor.visit(*this); }

bool StringNode::operator==(const Node& other) const {
  return typeid(*this) == typeid(other) &&
         value_ == static_cast<const StringNode&>(other).value_;
}

StringNode::StringNode(std::string value) : value_(std::move(value)) {}

const std::string& StringNode::get() const { return value_; }

}  // namespace json
