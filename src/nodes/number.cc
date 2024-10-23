#include "nodes/number.h"

#include "nodes/type.h"
#include "visitors/ret_visitor.h"
#include "visitors/visitor.h"

namespace json {

void Number::accept(Visitor& visitor) const { visitor.visit(*this); }

Node* Number::accept(ReturnVisitor& visitor) const {
  return visitor.visit(*this);
}

Number::Number(double value) : value_(value) {}

const double Number::get() const { return value_; }

const Type Number::type() const { return Type::NUMBER; }

}  // namespace json
