#include "nodes/null.h"

#include "visitors/ret_visitor.h"
#include "visitors/visitor.h"

namespace json {

void Null::accept(Visitor& visitor) const { visitor.visit(*this); }

Node* Null::accept(ReturnVisitor& visitor) const {
  return visitor.visit(*this);
}

}  // namespace json
