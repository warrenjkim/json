#include "json/value.h"

#include <cstddef>
#include <string>

#include "json/exception.h"
#include "nodes/boolean.h"
#include "nodes/node.h"
#include "nodes/null.h"
#include "nodes/string.h"
#include "visitors/boolean_visitor.h"
#include "visitors/null_visitor.h"
#include "visitors/string_visitor.h"

namespace json {

Value::Value() : node_(nullptr) {}

Value::Value(Node* node) : node_(node) {
  if (!node) {
    throw ParseException("Parsing failed.");
  }
}

Value::operator bool() const {
  visitors::BooleanVisitor visitor;
  node_->accept(visitor);

  return visitor.result();
}

Value::operator const char*() const {
  visitors::StringVisitor visitor;
  node_->accept(visitor);

  return visitor.result().c_str();
}

Value::operator nullptr_t() const {
  visitors::NullVisitor visitor;
  node_->accept(visitor);

  return visitor.result();
}

bool operator==(const Value& lhs, const Value& rhs) {
  return *lhs.node_ == *rhs.node_;
}

bool operator==(const Value& lhs, bool rhs) {
  return *lhs.node_ == Boolean(rhs);
}

bool operator==(bool lhs, const Value& rhs) { return rhs == lhs; }

bool operator==(const Value& lhs, const char* rhs) {
  return *lhs.node_ == String(rhs);
}

bool operator==(const char* lhs, const Value& rhs) { return rhs == lhs; }

bool operator==(const Value& lhs, nullptr_t) { return *lhs.node_ == Null(); }

bool operator==(nullptr_t lhs, const Value& rhs) { return rhs == lhs; }

}  // namespace json
