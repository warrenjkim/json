#include "types/boolean.h"

#include "visitors/ret_visitor.h"
#include "visitors/visitor.h"

namespace json {

void Boolean::accept(Visitor& visitor) const { visitor.visit(*this); }

Type* Boolean::accept(ReturnVisitor& visitor) const {
  return visitor.visit(*this);
}

Boolean::Boolean(const bool value) : value_(value) {}

const bool Boolean::get() const { return value_; }

}  // namespace json
