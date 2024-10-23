#include "nodes/array.h"

#include <vector>

#include "nodes/node.h"
#include "nodes/type.h"
#include "visitors/ret_visitor.h"
#include "visitors/visitor.h"

namespace json {

void Array::accept(Visitor& visitor) const { visitor.visit(*this); }

Node* Array::accept(ReturnVisitor& visitor) const {
  return visitor.visit(*this);
}

void Array::add(Node* element) { array_.push_back(element); }

const size_t Array::size() const { return array_.size(); }

const bool Array::empty() const { return array_.empty(); }

std::vector<Node*>& Array::get() { return array_; }

const std::vector<Node*>& Array::get() const { return array_; }

const Type Array::type() const { return Type::ARRAY; }

}  // namespace json
