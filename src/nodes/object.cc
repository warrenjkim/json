#include "nodes/object.h"

#include "utils/rbt.h"
#include "visitors/visitor.h"

namespace json {

void Object::accept(visitors::Visitor& visitor) { visitor.visit(*this); }

void Object::accept(visitors::ConstVisitor& visitor) const {
  visitor.visit(*this);
}

void Object::add(const std::string& key, Node* value) {
  properties_[key] = value;
}

const size_t Object::size() const { return properties_.size(); }

const bool Object::empty() const { return properties_.empty(); }

utils::Map<std::string, Node*>& Object::get() { return properties_; }

const utils::Map<std::string, Node*>& Object::get() const {
  return properties_;
}

}  // namespace json
