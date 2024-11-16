#pragma once

#include "json/exception.h"
#include "nodes/number.h"
#include "nodes/string.h"
#include "value.h"
#include "visitors/get_visitor.h"
#include "visitors/number_visitor.h"
#include "visitors/string_visitor.h"

namespace json {

template <ReasonableNumber T>
Value::operator T() const {
  visitors::NumberVisitor visitor;
  node_->accept(visitor);

  return static_cast<T>(visitor.result());
}

template <ReasonableString T>
Value::operator T() const {
  visitors::StringVisitor visitor;
  node_->accept(visitor);

  return T(visitor.result());
}

template <ReasonableNumber T>
bool operator==(const Value& lhs, const T& rhs) {
  return *lhs.node_ == Number(rhs);
}

template <ReasonableNumber T>
bool operator==(const T& lhs, const Value& rhs) {
  return rhs == lhs;
}

template <ReasonableString T>
bool operator==(const Value& lhs, const T& rhs) {
  return *lhs.node_ == String(rhs);
}

template <ReasonableString T>
bool operator==(const T& lhs, const Value& rhs) {
  return rhs == lhs;
}

// TODO(return reference when caching is implemented)
template <ReasonableInteger T>
Value Value::operator[](const T index) {
  if (!node_) {
    throw BadCastException("No value set.");
  }

  visitors::GetVisitor visitor(index);
  node_->accept(visitor);

  return Value(visitor.result());
}

// TODO(return reference when caching is implemented)
template <ReasonableString T>
Value Value::operator[](const T key) {
  if (!node_) {
    throw BadCastException("No value set.");
  }

  visitors::GetVisitor visitor(key);
  node_->accept(visitor);

  return Value(visitor.result());
}

}  // namespace json
