#include "types/string.h"

#include <string>

#include "visitors/ret_visitor.h"
#include "visitors/visitor.h"

namespace json {

void String::accept(Visitor& visitor) const { visitor.visit(*this); }

Type* String::accept(ReturnVisitor& visitor) const {
  return visitor.visit(*this);
}

String::String(std::string value) : value_(std::move(value)) {}

const std::string& String::get() const { return value_; }

}  // namespace json
