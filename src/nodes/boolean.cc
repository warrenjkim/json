#include "nodes/boolean.h"

#include "visitors/ret_visitor.h"
#include "visitors/visitor.h"

namespace json {

void Boolean::accept(visitors::Visitor& visitor) const { visitor.visit(*this); }

Node* Boolean::accept(visitors::ReturnVisitor& visitor) const {
  return visitor.visit(*this);
}

Boolean::Boolean(const bool value) : value_(value) {}

bool& Boolean::get() { return value_; }

const bool& Boolean::get() const { return value_; }

}  // namespace json
