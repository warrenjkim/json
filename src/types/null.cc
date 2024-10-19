#include "types/null.h"

#include "types/visitors/visitor.h"

namespace json {

void Null::accept(Visitor& visitor) const { visitor.visit(*this); }

}  // namespace json
