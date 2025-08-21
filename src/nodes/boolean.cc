#include "warren/internal/nodes/boolean.h"

namespace json {

namespace nodes {

Node* Boolean::clone() const { return new Boolean(value_); }

Boolean::Boolean(const bool value) : value_(value) {}

bool Boolean::get() { return value_; }

const bool Boolean::get() const { return value_; }

}  // namespace nodes

}  // namespace json
