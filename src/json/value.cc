#include "json/value.h"

#include <cstddef>
#include <optional>
#include <string>

#include "json/exception.h"
#include "nodes/boolean.h"
#include "nodes/node.h"
#include "nodes/null.h"
#include "nodes/string.h"
#include "visitors/array_visitor.h"
#include "visitors/boolean_visitor.h"
#include "visitors/const_iterator_visitor.h"
#include "visitors/iterator_visitor.h"
#include "visitors/null_visitor.h"
#include "visitors/object_visitor.h"
#include "visitors/string_visitor.h"
#include "visitors/stringify_visitor.h"

namespace json {

Value::Value() : node_(nullptr), parent_(nullptr) {}

Value::~Value() {
  if (node_ && !parent_) {
    delete node_;
    node_ = nullptr;
  }
}

Value::Value(Value&& other) : node_(other.node_), parent_(this) {
  other.node_ = nullptr;
  other.parent_ = nullptr;
  other.key_ = std::nullopt;
}

Value::Value(const Value& other)
    : node_(other.node_ ? other.node_->clone() : nullptr), parent_(nullptr) {}

Value& Value::operator=(Value&& other) {
  if (*this == other) {
    return *this;
  }

  node_ = other.node_;
  other.node_ = nullptr;

  // TODO(move to yet another visitor class, probably)
  if (other.key_) {
    try {
      visitors::ObjectVisitor visitor;

      // remove from other's AST
      other.parent_->node_->accept(visitor);
      visitor.result().remove(*other.key_);

      // add to current AST
      parent_->node_->accept(visitor);
      delete visitor.result()[*key_];
      visitor.result().insert(*key_, node_);
    } catch (BadCastException) {
      visitors::ArrayVisitor visitor;

      // null out other's AST
      other.parent_->node_->accept(visitor);
      visitor.result().erase(visitor.result().begin() + std::stoi(*other.key_));

      // move to current AST
      parent_->node_->accept(visitor);
      delete visitor.result()[std::stoi(*key_)];
      visitor.result()[std::stoi(*key_)] = node_;
    }
  }

  other.parent_ = nullptr;
  other.key_ = std::nullopt;

  return *this;
}

Value& Value::operator=(const Value& other) {
  if (*this == other) {
    return *this;
  }

  node_ = other.node_ ? other.node_->clone() : nullptr;

  return *this;
}

Value::Value(Node* node) : node_(node), parent_(nullptr) {}

Value::Value(const nullptr_t) : node_(new Null()), parent_(nullptr) {}

Value::Value(const bool value) : node_(new Boolean(value)), parent_(nullptr) {}

Value::Value(const char* value) : node_(new String(value)), parent_(nullptr) {}

void Value::push_back(const nullptr_t) {
  if (!node_) {
    node_ = new Array();
  }

  visitors::ArrayVisitor visitor;
  node_->accept(visitor);

  visitor.result().push_back(new Null());
}

void Value::push_back(const bool value) {
  if (!node_) {
    node_ = new Array();
  }

  visitors::ArrayVisitor visitor;
  node_->accept(visitor);

  visitor.result().push_back(new Boolean(value));
}

void Value::push_back(const char* value) {
  if (!node_) {
    node_ = new Array();
  }

  visitors::ArrayVisitor visitor;
  node_->accept(visitor);

  visitor.result().push_back(new String(value));
}

void Value::push_back(const Value& value) {
  if (!node_) {
    node_ = new Array();
  }

  visitors::ArrayVisitor visitor;
  node_->accept(visitor);

  if (!value.node_) {
    visitor.result().push_back(new Null());
  } else {
    visitor.result().push_back(value.node_->clone());
  }
}

void Value::insert(const std::string& key, const nullptr_t) {
  if (!node_) {
    node_ = new Object();
  }

  visitors::ObjectVisitor visitor;
  node_->accept(visitor);

  visitor.result().insert(key, new Null());
}

void Value::insert(const std::string& key, bool value) {
  if (!node_) {
    node_ = new Object();
  }

  visitors::ObjectVisitor visitor;
  node_->accept(visitor);

  visitor.result().insert(key, new Boolean(value));
}

void Value::insert(const std::string& key, const char* value) {
  if (!node_) {
    node_ = new Object();
  }

  visitors::ObjectVisitor visitor;
  node_->accept(visitor);

  visitor.result().insert(key, new String(value));
}

void Value::insert(const std::string& key, const Value& value) {
  if (!node_) {
    node_ = new Object();
  }

  visitors::ObjectVisitor visitor;
  node_->accept(visitor);

  if (!value.node_) {
    visitor.result().insert(key, new Null());
  } else {
    visitor.result().insert(key, value.node_->clone());
  }
}

Value::Iterator Value::begin() {
  Value::Iterator it(this);
  visitors::IteratorVisitor visitor =
      visitors::IteratorVisitor(it, visitors::IteratorVisitor::Operation::BEGIN)
          .init();
  node_->accept(visitor);

  return it;
}

Value::Iterator Value::end() {
  Value::Iterator it(this);
  visitors::IteratorVisitor visitor =
      visitors::IteratorVisitor(it, visitors::IteratorVisitor::Operation::END)
          .init();
  node_->accept(visitor);

  return it;
}

Value::ConstIterator Value::cbegin() {
  Value::ConstIterator cit(this);
  visitors::ConstIteratorVisitor visitor =
      visitors::ConstIteratorVisitor(
          cit, visitors::ConstIteratorVisitor::Operation::BEGIN)
          .init();
  node_->accept(visitor);

  return cit;
}

Value::ConstIterator Value::cend() {
  Value::ConstIterator cit(this);
  visitors::ConstIteratorVisitor visitor =
      visitors::ConstIteratorVisitor(
          cit, visitors::ConstIteratorVisitor::Operation::END)
          .init();
  node_->accept(visitor);

  return cit;
}

Value::operator bool() const {
  visitors::BooleanVisitor visitor;
  node_->accept(visitor);

  return visitor.result();
}

Value::operator nullptr_t() const {
  visitors::NullVisitor visitor;
  node_->accept(visitor);

  return visitor.result();
}

Value::operator const char*() const {
  visitors::StringVisitor visitor;
  node_->accept(visitor);

  return visitor.result().c_str();
}

Value& Value::operator[](const char* key) {
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

Value& Value::operator=(const bool value) {
  if (parent_) {
    visitors::SetVisitor visitor(&node_, new Boolean(value), *key_);
    parent_->node_->accept(visitor);
    parent_->cache_.remove(*key_);
  } else {
    delete node_;
    node_ = new Boolean(value);
  }

  return *this;
}

Value& Value::operator=(const char* value) {
  if (parent_) {
    visitors::SetVisitor visitor(&node_, new String(value), *key_);
    parent_->node_->accept(visitor);
    parent_->cache_.remove(*key_);
  } else {
    delete node_;
    node_ = new String(value);
  }

  return *this;
}

Value& Value::operator=(const nullptr_t value) {
  if (parent_) {
    visitors::SetVisitor visitor(&node_, new Null(), *key_);
    parent_->node_->accept(visitor);
    parent_->cache_.remove(*key_);
  } else {
    delete node_;
    node_ = new Null();
  }

  return *this;
}

const std::string Value::to_string() const {
  visitors::StringifyVisitor visitor;
  node_->accept(visitor);

  return visitor.result();
}

bool operator==(const Value& lhs, const bool rhs) {
  return lhs.node_ && *lhs.node_ == Boolean(rhs);
}

bool operator==(const Value& lhs, const char* rhs) {
  return lhs.node_ && *lhs.node_ == String(rhs);
}

bool operator==(const Value& lhs, const nullptr_t) {
  return lhs.node_ && *lhs.node_ == Null();
}

bool operator==(const Value& lhs, const Value& rhs) {
  return (!lhs.node_ && !rhs.node_) ||
         ((lhs.node_ && rhs.node_) && (*lhs.node_ == *rhs.node_));
}

bool operator==(const Value& lhs, const Array& rhs) {
  return lhs.node_ && *lhs.node_ == rhs;
}

bool operator==(const Value& lhs, const Object& rhs) {
  return lhs.node_ && *lhs.node_ == rhs;
}

}  // namespace json

namespace json {

Value::Iterator::~Iterator() {
  visitors::IteratorVisitor visitor =
      visitors::IteratorVisitor(*this,
                                visitors::IteratorVisitor::Operation::DESTROY)
          .init();
  value_->node_->accept(visitor);
}

Value::Iterator::Iterator(Value* value) : curr_(nullptr), value_(value) {}

Value::Iterator& Value::Iterator::operator++() {
  visitors::IteratorVisitor visitor =
      visitors::IteratorVisitor(*this,
                                visitors::IteratorVisitor::Operation::INCREMENT)
          .init();
  value_->node_->accept(visitor);

  return *this;
}

Value::Iterator Value::Iterator::operator++(int) {
  Iterator temp = *this;
  ++(*this);

  return temp;
}

Value::Iterator& Value::Iterator::operator--() {
  visitors::IteratorVisitor visitor =
      visitors::IteratorVisitor(*this,
                                visitors::IteratorVisitor::Operation::DECREMENT)
          .init();
  value_->node_->accept(visitor);

  return *this;
}

Value::Iterator Value::Iterator::operator--(int) {
  Iterator temp = *this;
  --(*this);

  return temp;
}

Value::Iterator::reference Value::Iterator::operator*() { return *curr_; }

Value::Iterator::pointer Value::Iterator::operator->() {
  return &(operator*());
}

bool Value::Iterator::operator==(const Iterator& other) const {
  return value_ == other.value_ && curr_->node_ == other.curr_->node_;
}

bool Value::Iterator::operator!=(const Iterator& other) const {
  return !(*this == other);
}

}  // namespace json

namespace json {

Value::ConstIterator::~ConstIterator() {
  visitors::ConstIteratorVisitor visitor =
      visitors::ConstIteratorVisitor(
          *this, visitors::ConstIteratorVisitor::Operation::DESTROY)
          .init();
  value_->node_->accept(visitor);
}

Value::ConstIterator::ConstIterator(Value* value)
    : curr_(nullptr), value_(value) {}

Value::ConstIterator& Value::ConstIterator::operator++() {
  visitors::ConstIteratorVisitor visitor =
      visitors::ConstIteratorVisitor(
          *this, visitors::ConstIteratorVisitor::Operation::INCREMENT)
          .init();
  value_->node_->accept(visitor);

  return *this;
}

Value::ConstIterator Value::ConstIterator::operator++(int) {
  ConstIterator temp = *this;
  ++(*this);

  return temp;
}

Value::ConstIterator& Value::ConstIterator::operator--() {
  visitors::ConstIteratorVisitor visitor =
      visitors::ConstIteratorVisitor(
          *this, visitors::ConstIteratorVisitor::Operation::DECREMENT)
          .init();
  value_->node_->accept(visitor);

  return *this;
}

Value::ConstIterator Value::ConstIterator::operator--(int) {
  ConstIterator temp = *this;
  --(*this);

  return temp;
}

Value::ConstIterator::reference Value::ConstIterator::operator*() const {
  return *curr_;
}

Value::ConstIterator::pointer Value::ConstIterator::operator->() const {
  return &(operator*());
}

bool Value::ConstIterator::operator==(const ConstIterator& other) const {
  return value_ == other.value_ && curr_->node_ == other.curr_->node_;
}

bool Value::ConstIterator::operator!=(const ConstIterator& other) const {
  return !(*this == other);
}

}  // namespace json
