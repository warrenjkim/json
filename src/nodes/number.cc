#include "warren/internal/nodes/number.h"

#include "warren/internal/dsa/numeric.h"
#include "warren/internal/visitors/visitor.h"

namespace json {

namespace nodes {

void Number::accept(visitors::Visitor& visitor) { visitor.visit(*this); }

void Number::accept(visitors::ConstVisitor& visitor) const {
  visitor.visit(*this);
}

Node* Number::clone() const { return new Number(value_); }

Number::Number(const dsa::Numeric& value) : value_(value) {}

dsa::Numeric Number::get() { return value_; }

const dsa::Numeric Number::get() const { return value_; }

}  // namespace nodes

}  // namespace json
