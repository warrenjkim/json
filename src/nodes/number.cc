#include "nodes/number.h"

#include "visitors/visitor.h"

namespace json {

void Number::accept(visitors::Visitor& visitor) { visitor.visit(*this); }

void Number::accept(visitors::ConstVisitor& visitor) const {
  visitor.visit(*this);
}

Number::Number(double value) : value_(value) {}

double Number::get() { return value_; }

const double Number::get() const { return value_; }

double* Number::ptr() { return &value_; }

const double* Number::ptr() const { return &value_; }

}  // namespace json
