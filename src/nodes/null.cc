#include "warren/internal/nodes/null.h"

#include <cstddef>

#include "warren/internal/visitors/visitor.h"

namespace json {

namespace nodes {

void Null::accept(visitors::Visitor& visitor) { visitor.visit(*this); }

void Null::accept(visitors::ConstVisitor& visitor) const {
  visitor.visit(*this);
}

Node* Null::clone() { return new Null(); }

Null::operator nullptr_t() const { return nullptr; }

}  // namespace nodes

}  // namespace json
