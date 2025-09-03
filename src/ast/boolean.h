#pragma once

#include "warren/json/node.h"
#include "warren/json/visitor.h"

namespace json {
namespace ast {

struct Boolean : public Node {
  void accept(Visitor& visitor) override { visitor.visit(this); }

  explicit Boolean(bool value) : value(value) {}

  bool value;
};

}  // namespace ast
}  // namespace json
