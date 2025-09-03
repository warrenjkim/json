#pragma once

#include <cstddef>

#include "warren/json/node.h"
#include "warren/json/visitor.h"

namespace json {
namespace ast {

struct Null : public Node {
  void accept(Visitor& visitor) override { visitor.visit(this); }

  explicit Null() : value(nullptr) {}

  nullptr_t value;
};

}  // namespace ast
}  // namespace json
