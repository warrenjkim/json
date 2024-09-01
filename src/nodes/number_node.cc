#include "nodes/number_node.h"

#include <cstdint>
#include <variant>

#include "visitors/visitor.h"

namespace json {

void NumberNode::accept(JsonVisitor& visitor) const { visitor.visit(*this); }

NumberNode::NumberNode(std::variant<int64_t, double> value)
    : value_(std::move(value)) {}

const std::variant<int64_t, double> NumberNode::get() const {
  return std::holds_alternative<int64_t>(value_) ? std::get<int64_t>(value_)
                                                 : std::get<double>(value_);
}

}  // namespace json
