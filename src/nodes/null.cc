#include "warren/internal/nodes/null.h"

#include <cstddef>

namespace json {

namespace nodes {

Node* Null::clone() const { return new Null(); }

Null::operator nullptr_t() const { return nullptr; }

}  // namespace nodes

}  // namespace json
