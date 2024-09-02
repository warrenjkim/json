#include "nodes/number_node.h"

#include <cmath>
#include <cstdint>
#include <limits>
#include <typeinfo>
#include <variant>

#include "visitors/visitor.h"

namespace json {

void NumberNode::accept(JsonVisitor& visitor) const { visitor.visit(*this); }

bool NumberNode::operator==(const Node& other) const {
  if (typeid(*this) != typeid(other)) {
    return false;
  }

  const auto& other_num = static_cast<const NumberNode&>(other);
  if (value_.index() != other_num.value_.index()) {
    return false;
  }

  if (std::holds_alternative<int64_t>(value_)) {
    return std::get<int64_t>(value_) == std::get<int64_t>(other_num.value_);
  }

  const double lhs = std::get<double>(value_);
  const double rhs = std::get<double>(other_num.value_);
  return std::abs(lhs - rhs) <= std::numeric_limits<double>::epsilon() *
                                    std::fmax(std::abs(lhs), std::abs(rhs));
}

NumberNode::NumberNode(std::variant<int64_t, double> value)
    : value_(std::move(value)) {}

const std::variant<int64_t, double> NumberNode::get() const {
  return std::holds_alternative<int64_t>(value_) ? std::get<int64_t>(value_)
                                                 : std::get<double>(value_);
}

}  // namespace json
