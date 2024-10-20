#include "visitors/cmp_visitor.h"

#include <cmath>

#include "types/array.h"
#include "types/boolean.h"
#include "types/null.h"
#include "types/number.h"
#include "types/object.h"
#include "types/string.h"
#include "types/type.h"

namespace json {

namespace visitors {

CmpVisitor::CmpVisitor(Type* root) : expected_(root), result_(true) {}

CmpVisitor::CmpVisitor(const Type* root)
    : expected_(const_cast<Type*>(root)), result_(true) {}

void CmpVisitor::visit(const Array& node) {
  auto expected = dynamic_cast<Array*>(expected_);
  if (!expected || expected->size() != node.size()) {
    result_ = false;
    return;
  }

  for (size_t i = 0; i < node.size(); ++i) {
    Type* expected_element = expected->get()[i];
    Type* actual_element = node.get()[i];
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

    Type* actual_value = node.get().find(key)->second;
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
