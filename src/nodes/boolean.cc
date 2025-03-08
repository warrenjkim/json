#include "nodes/boolean.h"

#include "visitors/visitor.h"

namespace json {

namespace nodes {

void Boolean::accept(visitors::Visitor& visitor) { visitor.visit(*this); }

void Boolean::accept(visitors::ConstVisitor& visitor) const {
  visitor.visit(*this);
}

Node* Boolean::clone() { return new Boolean(value_); }

Boolean::Boolean(const bool value) : value_(value) {}

bool Boolean::get() { return value_; }

const bool Boolean::get() const { return value_; }

}  // namespace nodes

}  // namespace json
