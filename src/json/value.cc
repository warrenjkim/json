#include "json/value.h"

#include <cstddef>
#include <string>
#include <vector>

#include "json/exception.h"
#include "nodes/node.h"
#include "visitors/set_visitor.h"

namespace json {

Value::Value() : data_(), type_(Type::JSON_NULL), node_(nullptr) {}

Value::Value(Node* node) : data_(), type_(Type::JSON_NULL), node_(node) {
  if (!node) {
    throw ParseException("Parsing failed.");
  }

  visitors::SetVisitor visitor(*this);
  node_->accept(visitor);
}

Value::operator bool() const {
  if (type_ != Type::BOOLEAN) {
    throw BadCastException("Value is not a boolean.");
  }

  return data_.boolean;
}

Value::operator const char*() const {
  if (type_ != Type::STRING) {
    throw BadCastException("Value is not a string.");
  }

  return data_.string->c_str();
}

Value::operator std::nullptr_t() const {
  if (type_ != Type::JSON_NULL) {
    throw BadCastException("Value is not a nullptr.");
  }

  return nullptr;
}

bool operator==(const Value& lhs, const Value& rhs) {
  if (lhs.type_ != rhs.type_) {
    return false;
  }

  switch (lhs.type_) {
    case Value::Type::BOOLEAN:
      return *lhs.data_.boolean == *rhs.data_.boolean;
    case Value::Type::NUMBER:
      return *lhs.data_.number == *rhs.data_.number;
    case Value::Type::STRING:
      return *lhs.data_.string == *rhs.data_.string;
    case Value::Type::ARRAY:
      return *lhs.data_.array == *rhs.data_.array;
    case Value::Type::OBJECT:
      return *lhs.data_.object == *rhs.data_.object;
    case Value::Type::JSON_NULL:
      return true;
  }
}

bool operator==(const Value& lhs, bool rhs) {
  return lhs.type_ == Value::Type::BOOLEAN && *lhs.data_.boolean == rhs;
}

bool operator==(bool lhs, const Value& rhs) { return rhs == lhs; }

bool operator==(const Value& lhs, const char* rhs) {
  return lhs.type_ == Value::Type::STRING && *lhs.data_.string == rhs;
}

bool operator==(const char* lhs, const Value& rhs) { return rhs == lhs; }

bool operator==(const Value& lhs, std::nullptr_t) {
  return lhs.type_ == Value::Type::JSON_NULL;
}

bool operator==(std::nullptr_t, const Value& rhs) {
  return rhs.type_ == Value::Type::JSON_NULL;
}

}  // namespace json
