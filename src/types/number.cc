#include "types/number.h"

#include "visitors/ret_visitor.h"
#include "visitors/visitor.h"

namespace json {

void Number::accept(Visitor& visitor) const { visitor.visit(*this); }

Type* Number::accept(ReturnVisitor& visitor) const {
  return visitor.visit(*this);
}

Number::Number(double value) : value_(value) {}

const double Number::get() const { return value_; }

}  // namespace json
