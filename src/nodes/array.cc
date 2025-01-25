#include "nodes/array.h"

#include <vector>

#include "nodes/node.h"
#include "visitors/visitor.h"

namespace json {

void Array::accept(visitors::Visitor& visitor) { visitor.visit(*this); }

void Array::accept(visitors::ConstVisitor& visitor) const {
  visitor.visit(*this);
}

Node* Array::clone() {
  Array* arr = new Array();
  for (Node* node : array_) {
    arr->add(node->clone());
  }

  return arr;
}

Array::Array(const std::vector<Node*>& array) : array_(array) {}

Array::~Array() {
  for (Node* node : array_) {
    delete node;
  }
}

void Array::add(Node* element) { array_.push_back(element); }

const size_t Array::size() const { return array_.size(); }

const bool Array::empty() const { return array_.empty(); }

std::vector<Node*>& Array::get() { return array_; }

const std::vector<Node*>& Array::get() const { return array_; }

}  // namespace json
