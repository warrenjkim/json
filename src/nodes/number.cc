#include "nodes/number.h"

#include "visitors/ret_visitor.h"
#include "visitors/visitor.h"

namespace json {

void Number::accept(visitors::Visitor& visitor) const { visitor.visit(*this); }

Node* Number::accept(visitors::ReturnVisitor& visitor) const {
  return visitor.visit(*this);
}

Number::Number(double value) : value_(value) {}

double& Number::get() { return value_; }

const double& Number::get() const { return value_; }

}  // namespace json
