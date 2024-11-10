#include "nodes/array.h"

#include <vector>

#include "nodes/node.h"
#include "visitors/visitor.h"

namespace json {

void Array::accept(visitors::Visitor& visitor) { visitor.visit(*this); }

void Array::accept(visitors::ConstVisitor& visitor) const {
  visitor.visit(*this);
}

void Array::add(Node* element) { array_.push_back(element); }

const size_t Array::size() const { return array_.size(); }

const bool Array::empty() const { return array_.empty(); }

std::vector<Node*>& Array::get() { return array_; }

const std::vector<Node*>& Array::get() const { return array_; }

std::vector<Node*>* Array::ptr() { return &array_; }

const std::vector<Node*>* Array::ptr() const { return &array_; }

}  // namespace json
