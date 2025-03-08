#include "json/value.h"

#include <cstddef>
#include <iterator>
#include <optional>
#include <string>

#include "json/exception.h"
#include "nodes/boolean.h"
#include "nodes/node.h"
#include "nodes/null.h"
#include "nodes/string.h"
#include "visitors/array_visitor.h"
#include "visitors/boolean_visitor.h"
#include "visitors/container_type_visitor.h"
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

  if (other.key_) {
    ContainerType type;
    visitors::ContainerTypeVisitor visitor(type);
    other.parent_->node_->accept(visitor);
    switch (type) {
      case ARRAY: {
        visitors::ArrayVisitor visitor;

        other.parent_->node_->accept(visitor);
        visitor.result().erase(visitor.result().begin() +
                               std::stoi(*other.key_));

        parent_->node_->accept(visitor);
        delete visitor.result()[std::stoi(*key_)];
        visitor.result()[std::stoi(*key_)] = node_;
      } break;
      case OBJECT: {
        visitors::ObjectVisitor visitor;

        other.parent_->node_->accept(visitor);
        visitor.result().erase(*other.key_);

        parent_->node_->accept(visitor);
        delete visitor.result()[*key_];
        visitor.result().insert(*key_, node_);
      } break;
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

bool Value::empty() const noexcept { return size() == 0; }

size_t Value::size() const noexcept {
  if (!node_) {
    return 0;
  }

  ContainerType type;
  visitors::ContainerTypeVisitor visitor(type);
  try {
    node_->accept(visitor);
  } catch (NonIterableTypeException) {
    return 0;
  }

  size_t size = 0;
  switch (type) {
    case ARRAY: {
      visitors::ArrayVisitor visitor;
      node_->accept(visitor);
      size = visitor.result().size();
    } break;
    case OBJECT: {
      visitors::ObjectVisitor visitor;
      node_->accept(visitor);
      size = visitor.result().size();
    } break;
  }

  return size;
}

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

void Value::erase(Value::ConstIterator position) {
  switch (position.type_) {
    case ContainerType::ARRAY:
      erase(std::stoi(*(position->key_)));
      break;
    case ContainerType::OBJECT:
      erase(*(position->key_));
      break;
  }
}

void Value::erase(Value::ConstIterator first, Value::ConstIterator last) {
  switch (first.type_) {
    case ContainerType::ARRAY: {
      int i = std::stoi(*(first->key_));
      int start = i;
      while (i < std::stoi(*(last->key_))) {
        erase(start);
        i++;
      }
    } break;
    case ContainerType::OBJECT: {
      ConstIterator it = first;
      while (it != last) {
        ConstIterator curr = it++;
        erase(*(curr->key_));
      }
    } break;
  }
}

Value::Iterator Value::begin() noexcept {
  return Iterator(this, Iterator::StartPosition::BEGIN);
}

Value::Iterator Value::end() noexcept {
  return Iterator(this, Iterator::StartPosition::END);
}

Value::ConstIterator Value::cbegin() noexcept {
  return ConstIterator(this, ConstIterator::StartPosition::CBEGIN);
}

Value::ConstIterator Value::cend() noexcept {
  return ConstIterator(this, ConstIterator::StartPosition::CEND);
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
    parent_->cache_.erase(*key_);
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
    parent_->cache_.erase(*key_);
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
    parent_->cache_.erase(*key_);
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

Value::Iterator::~Iterator() { ::operator delete(curr_); }

Value::Iterator::Iterator(const Iterator& other)
    : curr_(nullptr), value_(other.value_), type_(other.type_) {
  switch (type_) {
    case ContainerType::ARRAY:
      it_.array_it = other.it_.array_it;
      break;
    case ContainerType::OBJECT:
      it_.map_it = other.it_.map_it;
      break;
  }
}

Value::Iterator::Iterator(Iterator&& other) noexcept
    : curr_(nullptr), value_(other.value_), type_(other.type_) {
  switch (type_) {
    case ContainerType::ARRAY:
      it_.array_it = other.it_.array_it;
      break;
    case ContainerType::OBJECT:
      it_.map_it = other.it_.map_it;
      break;
  }

  other.curr_ = nullptr;
  other.value_ = nullptr;
}

Value::Iterator& Value::Iterator::operator=(const Iterator& other) {
  if (this == &other) {
    return *this;
  }

  curr_ = nullptr;
  type_ = other.type_;
  value_ = other.value_;
  switch (type_) {
    case ContainerType::ARRAY:
      it_.array_it = other.it_.array_it;
      break;
    case ContainerType::OBJECT:
      it_.map_it = other.it_.map_it;
      break;
  }

  return *this;
}

Value::Iterator& Value::Iterator::operator=(Iterator&& other) noexcept {
  if (this == &other) {
    return *this;
  }

  curr_ = other.curr_;
  type_ = other.type_;
  value_ = other.value_;
  switch (type_) {
    case ContainerType::ARRAY:
      it_.array_it = other.it_.array_it;
      break;
    case ContainerType::OBJECT:
      it_.map_it = other.it_.map_it;
      break;
  }

  other.curr_ = nullptr;
  other.value_ = nullptr;

  return *this;
}

Value::Iterator::Iterator(Value* value, const StartPosition pos)
    : curr_(nullptr), value_(value) {
  visitors::ContainerTypeVisitor visitor(type_);
  value_->node_->accept(visitor);

  visitors::ArrayVisitor array_visitor;
  visitors::ObjectVisitor object_visitor;
  switch (pos) {
    case StartPosition::BEGIN:
      switch (type_) {
        case ContainerType::ARRAY:
          value_->node_->accept(array_visitor);
          it_.array_it = array_visitor.result().begin();
          break;
        case ContainerType::OBJECT:
          value_->node_->accept(object_visitor);
          it_.map_it = object_visitor.result().begin();
          break;
      }
      break;
    case StartPosition::END:
      switch (type_) {
        case ContainerType::ARRAY:
          value_->node_->accept(array_visitor);
          it_.array_it = array_visitor.result().end();
          break;
        case ContainerType::OBJECT:
          value_->node_->accept(object_visitor);
          it_.map_it = object_visitor.result().end();
          break;
      }
      break;
  }
}

Value::Iterator& Value::Iterator::operator++() noexcept {
  switch (type_) {
    case ContainerType::ARRAY:
      ++it_.array_it;
      break;
    case ContainerType::OBJECT:
      ++it_.map_it;
      break;
  }

  return *this;
}

Value::Iterator Value::Iterator::operator++(int) noexcept {
  Iterator temp = *this;
  ++(*this);

  return temp;
}

Value::Iterator& Value::Iterator::operator--() noexcept {
  switch (type_) {
    case ContainerType::ARRAY:
      --it_.array_it;
      break;
    case ContainerType::OBJECT:
      --it_.map_it;
      break;
  }

  return *this;
}

Value::Iterator Value::Iterator::operator--(int) noexcept {
  Iterator temp = *this;
  --(*this);

  return temp;
}

Value::Iterator::reference Value::Iterator::operator*() noexcept {
  if (!curr_) {
    curr_ = (Value*)::operator new(sizeof(Value));
    new (curr_) Value();
  }

  curr_->~Value();
  switch (type_) {
    case ContainerType::ARRAY:
      new (curr_) Value(*it_.array_it);
      break;
    case ContainerType::OBJECT:
      new (curr_) Value(it_.map_it->second);
      break;
  }

  curr_->parent_ = value_;

  switch (type_) {
    case ContainerType::ARRAY: {
      visitors::ArrayVisitor visitor;
      value_->node_->accept(visitor);
      curr_->key_ =
          std::to_string(std::distance(visitor.result().begin(), it_.array_it));
    } break;
    case ContainerType::OBJECT:
      curr_->key_ = it_.map_it->first;
      break;
  }

  return *curr_;
}

Value::Iterator::pointer Value::Iterator::operator->() noexcept {
  return &(operator*());
}

bool Value::Iterator::operator==(const Iterator& other) const noexcept {
  if (value_ != other.value_ || type_ != other.type_) {
    return false;
  }

  switch (type_) {
    case ContainerType::ARRAY:
      return it_.array_it == other.it_.array_it;
    case ContainerType::OBJECT:
      return it_.map_it == other.it_.map_it;
  }

  return false;
}

bool Value::Iterator::operator!=(const Iterator& other) const noexcept {
  return !(*this == other);
}

}  // namespace json

namespace json {

Value::ConstIterator::~ConstIterator() { ::operator delete(curr_); }

Value::ConstIterator::ConstIterator(const ConstIterator& other)
    : curr_(nullptr), value_(other.value_), type_(other.type_) {
  switch (type_) {
    case ContainerType::ARRAY:
      cit_.array_cit = other.cit_.array_cit;
      break;
    case ContainerType::OBJECT:
      cit_.map_cit = other.cit_.map_cit;
      break;
  }
}

Value::ConstIterator::ConstIterator(ConstIterator&& other) noexcept
    : curr_(nullptr), value_(other.value_), type_(other.type_) {
  switch (type_) {
    case ContainerType::ARRAY:
      cit_.array_cit = other.cit_.array_cit;
      break;
    case ContainerType::OBJECT:
      cit_.map_cit = other.cit_.map_cit;
      break;
  }

  other.curr_ = nullptr;
  other.value_ = nullptr;
}

Value::ConstIterator& Value::ConstIterator::operator=(
    const ConstIterator& other) {
  if (this == &other) {
    return *this;
  }

  curr_ = nullptr;
  type_ = other.type_;
  value_ = other.value_;
  switch (type_) {
    case ContainerType::ARRAY:
      cit_.array_cit = other.cit_.array_cit;
      break;
    case ContainerType::OBJECT:
      cit_.map_cit = other.cit_.map_cit;
      break;
  }

  return *this;
}

Value::ConstIterator& Value::ConstIterator::operator=(
    ConstIterator&& other) noexcept {
  if (this == &other) {
    return *this;
  }

  curr_ = other.curr_;
  type_ = other.type_;
  value_ = other.value_;
  switch (type_) {
    case ContainerType::ARRAY:
      cit_.array_cit = other.cit_.array_cit;
      break;
    case ContainerType::OBJECT:
      cit_.map_cit = other.cit_.map_cit;
      break;
  }

  other.curr_ = nullptr;
  other.value_ = nullptr;

  return *this;
}

Value::ConstIterator::ConstIterator(const Value::Iterator& it)
    : curr_(nullptr), value_(it.value_) {
  type_ = it.type_;
  switch (type_) {
    case ContainerType::ARRAY:
      cit_.array_cit = it.it_.array_it;
      break;
    case ContainerType::OBJECT:
      cit_.map_cit = it.it_.map_it;
      break;
  }
}

Value::ConstIterator::ConstIterator(Value* value, const StartPosition pos)
    : curr_(nullptr), value_(value) {
  visitors::ContainerTypeVisitor visitor(type_);
  value_->node_->accept(visitor);

  visitors::ArrayVisitor array_visitor;
  visitors::ObjectVisitor object_visitor;
  switch (pos) {
    case StartPosition::CBEGIN:
      switch (type_) {
        case ContainerType::ARRAY:
          value_->node_->accept(array_visitor);
          cit_.array_cit = array_visitor.result().cbegin();
          break;
        case ContainerType::OBJECT:
          value_->node_->accept(object_visitor);
          cit_.map_cit = object_visitor.result().cbegin();
          break;
      }
      break;
    case StartPosition::CEND:
      switch (type_) {
        case ContainerType::ARRAY:
          value_->node_->accept(array_visitor);
          cit_.array_cit = array_visitor.result().cend();
          break;
        case ContainerType::OBJECT:
          value_->node_->accept(object_visitor);
          cit_.map_cit = object_visitor.result().cend();
          break;
      }
      break;
  }
}

Value::ConstIterator& Value::ConstIterator::operator++() noexcept {
  switch (type_) {
    case ContainerType::ARRAY:
      ++cit_.array_cit;
      break;
    case ContainerType::OBJECT:
      ++cit_.map_cit;
      break;
  }

  return *this;
}

Value::ConstIterator Value::ConstIterator::operator++(int) noexcept {
  ConstIterator temp = *this;
  ++(*this);

  return temp;
}

Value::ConstIterator& Value::ConstIterator::operator--() noexcept {
  switch (type_) {
    case ContainerType::ARRAY:
      --cit_.array_cit;
      break;
    case ContainerType::OBJECT:
      --cit_.map_cit;
      break;
  }

  return *this;
}

Value::ConstIterator Value::ConstIterator::operator--(int) noexcept {
  ConstIterator temp = *this;
  --(*this);

  return temp;
}

Value::ConstIterator::const_reference
Value::ConstIterator::operator*() noexcept {
  if (!curr_) {
    curr_ = (Value*)::operator new(sizeof(Value));
    new (curr_) Value();
  }

  curr_->~Value();
  switch (type_) {
    case ContainerType::ARRAY:
      new (curr_) Value(*cit_.array_cit);
      break;
    case ContainerType::OBJECT:
      new (curr_) Value(cit_.map_cit->second);
      break;
  }

  curr_->parent_ = value_;

  switch (type_) {
    case ContainerType::ARRAY: {
      visitors::ArrayVisitor visitor;
      value_->node_->accept(visitor);
      curr_->key_ = std::to_string(
          std::distance(visitor.result().cbegin(), cit_.array_cit));
    } break;
    case ContainerType::OBJECT:
      curr_->key_ = cit_.map_cit->first;
      break;
  }

  return *curr_;
}

Value::ConstIterator::const_pointer
Value::ConstIterator::operator->() noexcept {
  return &(operator*());
}

bool Value::ConstIterator::operator==(
    const ConstIterator& other) const noexcept {
  if (value_ != other.value_ || type_ != other.type_) {
    return false;
  }

  switch (type_) {
    case ContainerType::ARRAY:
      return cit_.array_cit == other.cit_.array_cit;
    case ContainerType::OBJECT:
      return cit_.map_cit == other.cit_.map_cit;
  }

  return false;
}

bool Value::ConstIterator::operator!=(
    const ConstIterator& other) const noexcept {
  return !(*this == other);
}

}  // namespace json
