#pragma once

#include "exception.h"
#include "utils/queue.h"
#include "value.h"
#include "visitors/get_visitor.h"

namespace json {

template <ReasonableNumber T>
Value::operator T() const {
  if (type_ != Type::NUMBER) {
    throw BadCastException("Value is not a number.");
  }

  return static_cast<T>(*data_.number);
}

template <ReasonableString T>
Value::operator T() const {
  if (type_ != Type::STRING) {
    throw BadCastException("Value is not a string");
  }

  return T(*data_.string);
}

template <ReasonableNumber T>
bool operator==(const Value& lhs, const T& rhs) {
  return lhs.type_ == Value::Type::NUMBER && *lhs.data_.number == rhs;
}

template <ReasonableNumber T>
bool operator==(const T& lhs, const Value& rhs) {
  return rhs == lhs;
}

template <ReasonableString T>
bool operator==(const Value& lhs, const T& rhs) {
  return lhs.type_ == Value::Type::STRING && *lhs.data_.string == rhs;
}

template <ReasonableString T>
bool operator==(const T& lhs, const Value& rhs) {
  return rhs == lhs;
}

// TODO(return reference when caching is implemented)
template <ReasonableInteger T>
Value Value::operator[](const T index) {
  if (!node_) {
    return *this;
  }

  visitors::GetVisitor visitor(
      utils::Queue<std::string>::of(std::to_string(index)));
  node_->accept(visitor);

  return Value(visitor.result());
}

// TODO(return reference when caching is implemented)
template <ReasonableString T>
Value Value::operator[](const T key) {
  if (!node_) {
    return *this;
  }

  utils::Queue<std::string> keys = utils::Queue<std::string>::of(key);
  visitors::GetVisitor visitor(keys);
  node_->accept(visitor);

  return Value(visitor.result());
}

}  // namespace json
