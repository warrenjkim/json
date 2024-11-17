#include "json/value.h"

#include <cstddef>
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

namespace json {

Value::Value() : node_(nullptr) {}

Value::Value(Node* node) : node_(node) {
  if (!node) {
    throw ParseException("Parsing failed.");
  }
}

void Value::add(bool value) {
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
    visitor.result().push_back(value.node_);
  }
}

void Value::add(nullptr_t) {
  if (!node_) {
    node_ = new Array();
  }

  visitors::ArrayVisitor visitor;
  node_->accept(visitor);

  visitor.result().push_back(new Null());
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
    visitor.result().insert(key, value.node_);
  }
}

void Value::put(const std::string& key, nullptr_t) {
  if (!node_) {
    node_ = new Object();
  }

  visitors::ObjectVisitor visitor;
  node_->accept(visitor);

  visitor.result().insert(key, new Null());
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

Value& Value::operator=(bool value) {
  delete node_;
  node_ = new Boolean(value);

  return *this;
}

Value& Value::operator=(const char* value) {
  delete node_;
  node_ = new String(value);

  return *this;
}

Value& Value::operator=(nullptr_t value) {
  delete node_;
  node_ = new Null();

  return *this;
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

bool operator==(const Value& lhs, const Array& rhs) {
  return *lhs.node_ == rhs;
}

bool operator==(const Array& lhs, const Value& rhs) { return rhs == lhs; }

bool operator==(const Value& lhs, const Object& rhs) {
  return *lhs.node_ == rhs;
}

bool operator==(const Object& lhs, const Value& rhs) { return rhs == lhs; }

}  // namespace json
