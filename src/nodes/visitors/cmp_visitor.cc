#include "nodes/visitors/cmp_visitor.h"

#include <cmath>

#include "nodes/array_node.h"
#include "nodes/boolean_node.h"
#include "nodes/null_node.h"
#include "nodes/number_node.h"
#include "nodes/object_node.h"
#include "nodes/string_node.h"

namespace json {

namespace nodes {

namespace visitors {

CmpVisitor::CmpVisitor(Node* root) : expected_(root), result_(true) {}

CmpVisitor::CmpVisitor(const Node* root)
    : expected_(const_cast<Node*>(root)), result_(true) {}

void CmpVisitor::visit(const ArrayNode& node) {
  if (!result_) {
    return;
  }
  auto expected = dynamic_cast<ArrayNode*>(expected_);
  if (!expected || expected->size() != node.size()) {
    result_ = false;
    return;
  }
  for (size_t i = 0; i < node.size(); ++i) {
    auto expected_element = expected->get()[i];
    auto actual_element = node.get()[i];
    CmpVisitor cmp_visitor(expected_element);
    actual_element->accept(cmp_visitor);
    if (!cmp_visitor.result()) {
      result_ = false;
      return;
    }
  }
}

void CmpVisitor::visit(const BooleanNode& node) {
  if (!result_) {
    return;
  }
  auto expected = dynamic_cast<BooleanNode*>(expected_);
  if (!expected || expected->get() != node.get()) {
    result_ = false;
  }
}

void CmpVisitor::visit(const NullNode& node) {
  if (!result_) {
    return;
  }
  auto expected = dynamic_cast<NullNode*>(expected_);
  if (!expected) {
    result_ = false;
  }
}

void CmpVisitor::visit(const NumberNode& node) {
  if (!result_) {
    return;
  }
  auto expected = dynamic_cast<NumberNode*>(expected_);
  if (!expected || std::abs(expected->get() - node.get()) > 1e-10) {
    result_ = false;
  }
}

void CmpVisitor::visit(const ObjectNode& node) {
  if (!result_) {
    return;
  }
  auto expected = dynamic_cast<ObjectNode*>(expected_);
  if (!expected || expected->size() != node.size()) {
    result_ = false;
    return;
  }
  for (const auto& [key, expected_value] : expected->get()) {
    if (!node.get().contains(key)) {
      result_ = false;
      return;
    }
    auto actual_value = node.get().find(key)->second;
    CmpVisitor cmp_visitor(expected_value);
    actual_value->accept(cmp_visitor);
    if (!cmp_visitor.result()) {
      result_ = false;
      return;
    }
  }
}

void CmpVisitor::visit(const StringNode& node) {
  if (!result_) {
    return;
  }
  auto expected = dynamic_cast<StringNode*>(expected_);
  if (!expected || expected->get() != node.get()) {
    result_ = false;
  }
}

bool CmpVisitor::result() const { return result_; }

bool operator==(const Node& lhs, const Node& rhs) {
  CmpVisitor cmp_visitor(const_cast<Node*>(&lhs));
  rhs.accept(cmp_visitor);

  return cmp_visitor.result();
}

}  // namespace visitors

}  // namespace nodes

}  // namespace json
