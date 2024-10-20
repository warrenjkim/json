#include "types/array.h"

#include <vector>

#include "types/type.h"
#include "visitors/ret_visitor.h"
#include "visitors/visitor.h"

namespace json {

void Array::accept(Visitor& visitor) const { visitor.visit(*this); }

Type* Array::accept(ReturnVisitor& visitor) const {
  return visitor.visit(*this);
}

void Array::add(Type* element) { array_.push_back(element); }

const size_t Array::size() const { return array_.size(); }

const bool Array::empty() const { return array_.empty(); }

std::vector<Type*>& Array::get() { return array_; }

const std::vector<Type*>& Array::get() const { return array_; }

}  // namespace json
