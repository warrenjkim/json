#include "warren/internal/nodes/number.h"

#include "warren/internal/dsa/numeric.h"

namespace json {

namespace nodes {

Node* Number::clone() const { return new Number(value_); }

Number::Number(const dsa::Numeric& value) : value_(value) {}

dsa::Numeric Number::get() { return value_; }

const dsa::Numeric Number::get() const { return value_; }

}  // namespace nodes

}  // namespace json
