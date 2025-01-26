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
      visitor.result().insert(*key_, node_);
    } catch (BadCastException) {
      visitors::ArrayVisitor visitor;

      // null out other's AST
      other.parent_->node_->accept(visitor);
      visitor.result()[std::stoi(*other.key_)] = new Null();

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

Value::Value(Node* node) : node_(node), parent_(nullptr) {
  if (!node) {
    throw ParseException("Parsing failed.");
  }
}

Value::Value(const nullptr_t) : node_(new Null()), parent_(nullptr) {}

Value::Value(const bool value) : node_(new Boolean(value)), parent_(nullptr) {}

Value::Value(const char* value) : node_(new String(value)), parent_(nullptr) {}

void Value::add(const nullptr_t) {
  if (!node_) {
    node_ = new Array();
  }

  visitors::ArrayVisitor visitor;
  node_->accept(visitor);

  visitor.result().push_back(new Null());
}

void Value::add(const bool value) {
  if (!node_) {
    node_ = new Array();
  }

  visitors::ArrayVisitor visitor;
  node_->accept(visitor);

  visitor.result().push_back(new Boolean(value));
}

void Value::add(const char* value) {
  if (!node_) {
    node_ = new Array();
  }

  visitors::ArrayVisitor visitor;
  node_->accept(visitor);

  visitor.result().push_back(new String(value));
}

void Value::add(const Value& value) {
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

void Value::put(const std::string& key, const nullptr_t) {
  if (!node_) {
    node_ = new Object();
  }

  visitors::ObjectVisitor visitor;
  node_->accept(visitor);

  visitor.result().insert(key, new Null());
}

void Value::put(const std::string& key, bool value) {
  if (!node_) {
    node_ = new Object();
  }

  visitors::ObjectVisitor visitor;
  node_->accept(visitor);

  visitor.result().insert(key, new Boolean(value));
}

void Value::put(const std::string& key, const char* value) {
  if (!node_) {
    node_ = new Object();
  }

  visitors::ObjectVisitor visitor;
  node_->accept(visitor);

  visitor.result().insert(key, new String(value));
}

void Value::put(const std::string& key, const Value& value) {
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

Value& Value::operator=(const bool value) {
  if (parent_ && key_) {
    parent_->cache_.remove(*key_);
  }

  if (parent_) {
    visitors::SetVisitor visitor(&node_, new Boolean(value), *key_);
    parent_->node_->accept(visitor);
  } else {
    delete node_;
    node_ = new Boolean(value);
  }

  return *this;
}

Value& Value::operator=(const char* value) {
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

Value& Value::operator=(const nullptr_t value) {
  if (parent_ && key_) {
    parent_->cache_.remove(*key_);
  }

  if (parent_) {
    visitors::SetVisitor visitor(&node_, new Null(), *key_);
    parent_->node_->accept(visitor);
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
