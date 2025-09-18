#pragma once

#include <cstdint>

#include "warren/json/internal/ast/node.h"
#include "warren/json/internal/ast/visitor.h"

namespace json {
namespace ast {

struct Number : public Node {
  void accept(Visitor& visitor) const override { visitor.visit(*this); }

  explicit Number(double value) : dbl(value), type(Type::DOUBLE) {}

  explicit Number(int32_t value) : intgr(value), type(Type::INTEGRAL) {}

  union {
    double dbl;
    int32_t intgr;
  };

  enum Type { DOUBLE, INTEGRAL } type;
};

}  // namespace ast
}  // namespace json
