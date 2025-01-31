#pragma once

#include <string>

#include "exception.h"
#include "json/exception.h"
#include "nodes/array.h"
#include "nodes/null.h"
#include "nodes/number.h"
#include "nodes/object.h"
#include "nodes/string.h"
#include "value.h"
#include "visitors/array_visitor.h"
#include "visitors/get_visitor.h"
#include "visitors/number_visitor.h"
#include "visitors/object_visitor.h"
#include "visitors/set_visitor.h"
#include "visitors/string_visitor.h"

namespace json {

template <ReasonableNumber T>
Value::Value(const T value) : node_(new Number(value)), parent_(nullptr) {}

template <ReasonableString T>
Value::Value(const T& value) : node_(new String(value)), parent_(nullptr) {}

template <ReasonableNumber T>
void Value::push_back(const T value) {
  if (!node_) {
    node_ = new Array();
  }

  visitors::ArrayVisitor visitor;
  node_->accept(visitor);

  visitor.result().push_back(new Number(value));
}

template <ReasonableString T>
void Value::push_back(const T& value) {
  if (!node_) {
    node_ = new Array();
  }
  visitors::ArrayVisitor visitor;
  node_->accept(visitor);

  visitor.result().push_back(new String(value));
}

template <ReasonableNumber T>
void Value::put(const std::string& key, const T value) {
  if (!node_) {
    node_ = new Object();
  }

  visitors::ObjectVisitor visitor;
  node_->accept(visitor);

  visitor.result().insert(key, new Number(value));
}

template <ReasonableString T>
void Value::put(const std::string& key, const T& value) {
  if (!node_) {
    node_ = new Object();
  }
  visitors::ObjectVisitor visitor;
  node_->accept(visitor);

  visitor.result().insert(key, new String(value));
}

template <ReasonableInteger T>
void Value::remove(const T index) {
  cache_.remove(std::to_string(index));
  visitors::ArrayVisitor visitor;
  node_->accept(visitor);
  visitor.result()[index] = nullptr;
}

template <ReasonableString T>
void Value::remove(const T key) {
  cache_.remove(key);
  visitors::ObjectVisitor visitor;
  node_->accept(visitor);
  visitor.result().remove(key);
}

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
  cache_.insert(key, Value());
  cache_[key].node_ = visitor.result();
  cache_[key].parent_ = this;
  cache_[key].key_ = key;

  return cache_[key];
}

template <ReasonableString T>
Value& Value::operator[](const T& key) {
  if (!node_ || (parent_ && *node_ == Null())) {
    node_ = new Object();
  }

  if (cache_.contains(key)) {
    return cache_[key];
  }

  visitors::GetVisitor visitor(key);
  node_->accept(visitor);
  cache_.insert(key, Value());
  cache_[key].node_ = visitor.result();
  cache_[key].parent_ = this;
  cache_[key].key_ = key;

  return cache_[key];
}

template <ReasonableNumber T>
Value& Value::operator=(const T value) {
  if (parent_ && key_) {
    parent_->cache_.remove(*key_);
  }

  if (parent_) {
    visitors::SetVisitor visitor(&node_, new Number(value), *key_);
    parent_->node_->accept(visitor);
  } else {
    delete node_;
    node_ = new Number(value);
  }

  return *this;
}

template <ReasonableString T>
Value& Value::operator=(const T& value) {
  if (parent_ && key_) {
    parent_->cache_.remove(*key_);
  }

  if (parent_) {
    visitors::SetVisitor visitor(&node_, new String(value), *key_);
    parent_->node_->accept(visitor);
  } else {
    delete node_;
    node_ = new String(value);
  }

  return *this;
}

template <ReasonableNumber T>
bool operator==(const Value& lhs, const T rhs) {
  return lhs.node_ && *lhs.node_ == Number(rhs);
}

template <ReasonableString T>
bool operator==(const Value& lhs, const T& rhs) {
  return lhs.node_ && *lhs.node_ == String(rhs);
}

}  // namespace json
