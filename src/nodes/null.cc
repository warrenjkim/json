#include "nodes/null.h"

#include <cstddef>

#include "visitors/visitor.h"

namespace json {

void Null::accept(visitors::Visitor& visitor) { visitor.visit(*this); }

void Null::accept(visitors::ConstVisitor& visitor) const {
  visitor.visit(*this);
}

Node* Null::clone() { return new Null(); }

Null::operator nullptr_t() const { return nullptr; }

}  // namespace json
