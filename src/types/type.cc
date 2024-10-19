#include "types/type.h"

#include "visitors/cmp_visitor.h"

namespace json {

const bool Type::operator==(const Type& other) const {
  visitors::CmpVisitor cmp_visitor(this);
  other.accept(cmp_visitor);

  return cmp_visitor.result();
}

const bool Type::operator!=(const Type& other) const {
  return !(*this == other);
}

}  // namespace json
