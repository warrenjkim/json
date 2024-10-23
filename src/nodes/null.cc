#include "nodes/null.h"

#include "nodes/type.h"
#include "visitors/ret_visitor.h"
#include "visitors/visitor.h"

namespace json {

void Null::accept(Visitor& visitor) const { visitor.visit(*this); }

Node* Null::accept(ReturnVisitor& visitor) const {
  return visitor.visit(*this);
}

const Type Null::type() const { return Type::JSON_NULL; }

}  // namespace json
