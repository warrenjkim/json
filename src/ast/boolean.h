#pragma once

#include "warren/json/internal/ast/node.h"
#include "warren/json/internal/ast/visitor.h"

namespace json {
namespace ast {

struct Boolean : public Node {
  void accept(Visitor& visitor) const override { visitor.visit(*this); }

  explicit Boolean(bool value) : value(value) {}

  bool value;
};

}  // namespace ast
}  // namespace json
