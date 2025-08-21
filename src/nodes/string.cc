#include "warren/internal/nodes/string.h"

#include <string>

namespace json {

namespace nodes {

Node* String::clone() const { return new String(value_); }

String::String(std::string value) : value_(std::move(value)) {}

std::string& String::get() { return value_; }

const std::string& String::get() const { return value_; }

}  // namespace nodes

}  // namespace json
