#include "visitors/cmp_visitor.h"

#include <cmath>
#include <optional>
#include <vector>

#include "utils/exception.h"
#include "nodes/array.h"
#include "nodes/boolean.h"
#include "nodes/node.h"
#include "nodes/null.h"
#include "nodes/number.h"
#include "nodes/object.h"
#include "nodes/string.h"
#include "visitors/array_visitor.h"
#include "visitors/boolean_visitor.h"
#include "visitors/null_visitor.h"
#include "visitors/number_visitor.h"
#include "visitors/object_visitor.h"
#include "visitors/string_visitor.h"

namespace json {

namespace visitors {

CmpVisitor::CmpVisitor(nodes::Node* lhs) : lhs_(lhs), result_(true) {}

CmpVisitor::CmpVisitor(const nodes::Node* lhs)
    : lhs_(const_cast<nodes::Node*>(lhs)), result_(true) {}

void CmpVisitor::visit(const nodes::Array& rhs) {
  ArrayVisitor visitor;
  try {
    lhs_->accept(visitor);
  } catch (BadCastException) {
    result_ = false;
    return;
  }

  std::vector<nodes::Node*>& lhs = visitor.result();
  if (lhs.size() != rhs.get().size()) {
    result_ = false;
    return;
  }

  for (size_t i = 0; i < lhs.size(); i++) {
    nodes::Node* l = lhs.at(i);
    nodes::Node* r = rhs.get().at(i);

    if (!l != !r) {
      result_ = false;
      return;
    }

    if (l && r) {
      CmpVisitor cmp_visitor(l);
      r->accept(cmp_visitor);

      if (!cmp_visitor.result()) {
        result_ = false;
        return;
      }
    }
  }
}

void CmpVisitor::visit(const nodes::Boolean& rhs) {
  BooleanVisitor visitor;
  try {
    lhs_->accept(visitor);
  } catch (BadCastException) {
    result_ = false;
    return;
  }

  result_ = visitor.result() == rhs.get();
}

void CmpVisitor::visit(const nodes::Null& rhs) {
  NullVisitor visitor;
  try {
    lhs_->accept(visitor);
  } catch (BadCastException) {
    result_ = false;
    return;
  }

  result_ = visitor.result() == rhs;
}

void CmpVisitor::visit(const nodes::Number& rhs) {
  NumberVisitor visitor;
  try {
    lhs_->accept(visitor);
  } catch (BadCastException) {
    result_ = false;
    return;
  }

  result_ = std::abs(visitor.result() - rhs.get()) < 1e-10;
}

void CmpVisitor::visit(const nodes::Object& rhs) {
  ObjectVisitor visitor;
  try {
    lhs_->accept(visitor);
  } catch (BadCastException) {
    result_ = false;
    return;
  }

  utils::Map<std::string, nodes::Node*>& lhs = visitor.result();
  if (lhs.size() != rhs.get().size()) {
    result_ = false;
    return;
  }

  for (const auto& [key, l] : lhs) {
    std::optional<nodes::Node*> r = rhs.get().at(key);
    if (!r) {
      result_ = false;
      return;
    }

    if (!l != !*r) {
      result_ = false;
      return;
    }

    if (l && *r) {
      CmpVisitor cmp_visitor(l);
      (*r)->accept(cmp_visitor);
      if (!cmp_visitor.result()) {
        result_ = false;
        return;
      }
    }
  }
}

void CmpVisitor::visit(const nodes::String& rhs) {
  StringVisitor visitor;
  try {
    lhs_->accept(visitor);
  } catch (BadCastException) {
    result_ = false;
    return;
  }

  result_ = visitor.result() == rhs.get();
}

bool CmpVisitor::result() const { return result_; }

}  // namespace visitors

}  // namespace json
