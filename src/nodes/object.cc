#include "nodes/object.h"

#include <string>

#include "utils/map.h"
#include "visitors/visitor.h"

namespace json {

namespace nodes {

void Object::accept(visitors::Visitor& visitor) { visitor.visit(*this); }

void Object::accept(visitors::ConstVisitor& visitor) const {
  visitor.visit(*this);
}

Node* Object::clone() {
  Object* obj = new Object();
  for (auto& [key, value] : properties_) {
    obj->insert(key, value->clone());
  }

  return obj;
}

Object::Object(const utils::Map<std::string, Node*>& properties) {
  for (auto& [key, value] : properties) {
    properties_[key] = value->clone();
  }
}

Object::~Object() {
  for (auto& [_, node] : properties_) {
    delete node;
  }
}

void Object::insert(const std::string& key, Node* value) {
  properties_.insert(key, value);
}

const size_t Object::size() const { return properties_.size(); }

const bool Object::empty() const { return properties_.empty(); }

utils::Map<std::string, Node*>& Object::get() { return properties_; }

const utils::Map<std::string, Node*>& Object::get() const {
  return properties_;
}

}  // namespace nodes

}  // namespace json
