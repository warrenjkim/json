#include "nodes/object.h"

#include <string>

#include "utils/rbt.h"
#include "visitors/visitor.h"

namespace json {

void Object::accept(visitors::Visitor& visitor) { visitor.visit(*this); }

void Object::accept(visitors::ConstVisitor& visitor) const {
  visitor.visit(*this);
}

Node* Object::clone() {
  Object* obj = new Object();
  for (auto [key, value] : properties_) {
    obj->add(key, value->clone());
  }

  return obj;
}

Object::Object(const utils::Map<std::string, Node*>& properties)
    : properties_(properties) {}

Object::~Object() {
  for (auto [_, node] : properties_) {
    delete node;
  }
}

void Object::add(const std::string& key, Node* value) {
  properties_[key] = value;
}

void Object::put(const std::string& key, Node* value) {
  properties_.insert(key, value);
}

const size_t Object::size() const { return properties_.size(); }

const bool Object::empty() const { return properties_.empty(); }

utils::Map<std::string, Node*>& Object::get() { return properties_; }

const utils::Map<std::string, Node*>& Object::get() const {
  return properties_;
}

utils::Map<std::string, Node*>* Object::ptr() { return &properties_; }

const utils::Map<std::string, Node*>* Object::ptr() const {
  return &properties_;
}

}  // namespace json
