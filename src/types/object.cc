#include "types/object.h"

#include "types/visitors/visitor.h"
#include "utils/rbt.h"

namespace json {

Object::~Object() {}

void Object::accept(Visitor& visitor) const { visitor.visit(*this); }

void Object::add(const std::string& key, Type* value) {
  properties_[key] = value;
}

const utils::Map<std::string, Type*>& Object::get() const {
  return properties_;
}

const size_t Object::size() const { return properties_.size(); }

const bool Object::empty() const { return properties_.empty(); }

}  // namespace json
