#include "types/string.h"

#include <string>

#include "types/visitors/visitor.h"

namespace json {

void String::accept(Visitor& visitor) const { visitor.visit(*this); }

String::String(std::string value) : value_(std::move(value)) {}

const std::string& String::get() const { return value_; }

}  // namespace json
