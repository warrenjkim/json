#include "types/boolean.h"

#include "types/visitors/visitor.h"

namespace json {

void Boolean::accept(Visitor& visitor) const { visitor.visit(*this); }

Boolean::Boolean(const bool value) : value_(value) {}

const bool Boolean::get() const { return value_; }

}  // namespace json
