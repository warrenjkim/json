#include "nodes/number_node.h"

#include <cmath>
#include <limits>
#include <typeinfo>

#include "visitors/visitor.h"

namespace json {

void NumberNode::accept(JsonVisitor& visitor) const { visitor.visit(*this); }

bool NumberNode::operator==(const Node& other) const {
  if (typeid(*this) != typeid(other)) {
    return false;
  }
  const auto& other_num = static_cast<const NumberNode&>(other);
  return std::abs(value_ - other_num.value_) <=
         std::numeric_limits<double>::epsilon() *
             std::fmax(std::abs(value_), std::abs(other_num.value_));
}

NumberNode::NumberNode(double value) : value_(value) {}

const double NumberNode::get() const { return value_; }

}  // namespace json
