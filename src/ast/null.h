#pragma once

#include <cstddef>

#include "node.h"
#include "visitor.h"

namespace json {
namespace ast {

struct Null : public Node {
  void accept(Visitor& visitor) const override { visitor.visit(*this); }

  explicit Null() : value(nullptr) {}

  nullptr_t value;
};

}  // namespace ast
}  // namespace json
