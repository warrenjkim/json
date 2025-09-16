#pragma once

#include <cstdint>

#include "node.h"
#include "visitor.h"

namespace json {
namespace ast {

struct Number : public Node {
  void accept(Visitor& visitor) const override { visitor.visit(*this); }

  explicit Number(double value) : dbl(value), type(Type::DOUBLE) {
    // TODO(fix this)
    this->value = value;
  }

  explicit Number(int32_t value) : dbl(value), type(Type::DOUBLE) {
    // TODO(fix this)
    this->value = double(value);
  }

  union {
    double dbl;
    int32_t intgr;
  };

  // TODO(remove this)
  double value;

  enum Type { INTEGRAL, DOUBLE } type;
};

}  // namespace ast
}  // namespace json
