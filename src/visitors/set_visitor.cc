#include "visitors/set_visitor.h"

#include "nodes/array.h"
#include "nodes/boolean.h"
#include "nodes/node.h"
#include "nodes/null.h"
#include "nodes/number.h"
#include "nodes/object.h"
#include "nodes/string.h"

namespace json {

namespace visitors {

SetVisitor::SetVisitor(Node* node, Node* value, std::optional<size_t> index)
    : node_(node), value_(value), index_(index) {}

void SetVisitor::visit(Array& node) {
  Node* old = node.get().at(*index_);
  node.get()[*index_] = value_;
  delete old;
}

void SetVisitor::visit(Boolean& node) {
  node_->~Node();
  new (node_) Boolean(node.get());
}

void SetVisitor::visit(Null& node) {
  node_->~Node();
  new (node_) Null();
}

void SetVisitor::visit(Number& node) {
  node_->~Node();
  new (node_) Number(node.get());
}

void SetVisitor::visit(Object& node) { value_->accept(*this); }

void SetVisitor::visit(String& node) {
  node_->~Node();
  new (node_) String(node.get());
}

}  // namespace visitors

}  // namespace json
