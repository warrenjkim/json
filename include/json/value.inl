#pragma once

#include <string>

#include "exception.h"
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
Value& Value::operator=(T value) {
  delete node_;
  node_ = new Number(value);
  return *this;
}

template <ReasonableString T>
Value& Value::operator=(T value) {
  delete node_;
  node_ = new String(value);
  return *this;
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

template <ReasonableInteger T>
Value& Value::operator[](const T index) {
  if (!node_) {
    throw BadCastException("No value set.");
  }

  std::string key = std::to_string(index);
  if (cache_.contains(key)) {
    return cache_[key];
  }

  visitors::GetVisitor visitor(index);
  node_->accept(visitor);
  cache_.insert(key, Value(visitor.result()));

  return cache_[key];
}

template <ReasonableString T>
Value& Value::operator[](const T key) {
  if (!node_) {
    throw BadCastException("No value set.");
  }

  if (cache_.contains(key)) {
    return cache_[key];
  }

  visitors::GetVisitor visitor(key);
  node_->accept(visitor);
  cache_.insert(key, Value(visitor.result()));

  return cache_[key];
}

}  // namespace json
