#include "warren/internal/nodes/object.h"

#include <map>
#include <string>

namespace json {

namespace nodes {

Node* Object::clone() const {
  Object* obj = new Object();
  for (auto& [key, value] : properties_) {
    obj->insert(key, value->clone());
  }

  return obj;
}

Object::Object(const std::map<std::string, Node*>& properties) {
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
  properties_.insert({key, value});
}

const size_t Object::size() const { return properties_.size(); }

const bool Object::empty() const { return properties_.empty(); }

std::map<std::string, Node*>& Object::get() { return properties_; }

const std::map<std::string, Node*>& Object::get() const { return properties_; }

}  // namespace nodes

}  // namespace json
