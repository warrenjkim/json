#include "nodes/string.h"

#include <string>

#include "nodes/type.h"
#include "visitors/ret_visitor.h"
#include "visitors/visitor.h"

namespace json {

void String::accept(Visitor& visitor) const { visitor.visit(*this); }

Node* String::accept(ReturnVisitor& visitor) const {
  return visitor.visit(*this);
}

String::String(std::string value) : value_(std::move(value)) {}

const std::string& String::get() const { return value_; }

const Type String::type() const { return Type::STRING; }

}  // namespace json
