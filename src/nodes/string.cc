#include "nodes/string.h"

#include <string>

#include "visitors/visitor.h"

namespace json {

void String::accept(visitors::Visitor& visitor) { visitor.visit(*this); }

void String::accept(visitors::ConstVisitor& visitor) const {
  visitor.visit(*this);
}

Node* String::clone() { return new String(value_); }

String::String(std::string value) : value_(std::move(value)) {}

std::string& String::get() { return value_; }

const std::string& String::get() const { return value_; }

}  // namespace json
