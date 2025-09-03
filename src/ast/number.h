#pragma once

#include "warren/json/node.h"
#include "warren/json/visitor.h"

namespace json {
namespace ast {

struct Number : public Node {
  void accept(Visitor& visitor) override { visitor.visit(this); }

  explicit Number(double value) : value(value) {}

  double value;
};

}  // namespace ast
}  // namespace json
