#include "nodes/boolean.h"

#include "nodes/type.h"
#include "visitors/ret_visitor.h"
#include "visitors/visitor.h"

namespace json {

void Boolean::accept(Visitor& visitor) const { visitor.visit(*this); }

Node* Boolean::accept(ReturnVisitor& visitor) const {
  return visitor.visit(*this);
}

Boolean::Boolean(const bool value) : value_(value) {}

const bool Boolean::get() const { return value_; }

const Type Boolean::type() const { return Type::BOOLEAN; }

}  // namespace json
