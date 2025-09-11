#pragma once

#include "node.h"
#include "visitor.h"

namespace json {
namespace ast {

struct Number : public Node {
  void accept(Visitor& visitor) const override { visitor.visit(*this); }

  explicit Number(double value) : value(value) {}

  double value;
};

}  // namespace ast
}  // namespace json
