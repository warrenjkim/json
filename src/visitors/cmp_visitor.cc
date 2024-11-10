#include "visitors/cmp_visitor.h"

#include <cmath>

#include "nodes/array.h"
#include "nodes/boolean.h"
#include "nodes/node.h"
#include "nodes/null.h"
#include "nodes/number.h"
#include "nodes/object.h"
#include "nodes/string.h"

namespace json {

namespace visitors {

CmpVisitor::CmpVisitor(Node* root) : expected_(root), result_(true) {}

CmpVisitor::CmpVisitor(const Node* root)
    : expected_(const_cast<Node*>(root)), result_(true) {}

void CmpVisitor::visit(const Array& node) {
  auto expected = dynamic_cast<Array*>(expected_);
  if (!expected || expected->size() != node.size()) {
    result_ = false;
    return;
  }

  for (size_t i = 0; i < node.size(); ++i) {
    Node* expected_element = expected->get().at(i);
    Node* actual_element = node.get().at(i);
    CmpVisitor cmp_visitor(expected_element);
    actual_element->accept(cmp_visitor);
    if (!cmp_visitor.result()) {
      result_ = false;
      return;
    }
  }
}

void CmpVisitor::visit(const Boolean& node) {
  auto expected = dynamic_cast<Boolean*>(expected_);
  if (!expected || expected->get() != node.get()) {
    result_ = false;
  }
}

void CmpVisitor::visit(const Null& node) {
  auto expected = dynamic_cast<Null*>(expected_);
  if (!expected) {
    result_ = false;
  }
}

void CmpVisitor::visit(const Number& node) {
  auto expected = dynamic_cast<Number*>(expected_);
  if (!expected || std::abs(expected->get() - node.get()) > 1e-10) {
    result_ = false;
  }
}

void CmpVisitor::visit(const Object& node) {
  auto expected = dynamic_cast<Object*>(expected_);
  if (!expected || expected->size() != node.size()) {
    result_ = false;
    return;
  }

  for (const auto& [key, expected_value] : expected->get()) {
    if (!node.get().contains(key)) {
      result_ = false;
      return;
    }

    Node* actual_value = node.get().find(key)->second;
    CmpVisitor cmp_visitor(expected_value);
    actual_value->accept(cmp_visitor);
    if (!cmp_visitor.result()) {
      result_ = false;
      return;
    }
  }
}

void CmpVisitor::visit(const String& node) {
  auto expected = dynamic_cast<String*>(expected_);
  if (!expected || expected->get() != node.get()) {
    result_ = false;
  }
}

bool CmpVisitor::result() const { return result_; }

}  // namespace visitors

}  // namespace json
