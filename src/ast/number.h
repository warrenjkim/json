#pragma once

#include "warren/json/node.h"

namespace json {
namespace ast {

struct Number : public Node {
  explicit Number(double value) : value(value) {}

  double value;
};

}  // namespace ast
}  // namespace json
