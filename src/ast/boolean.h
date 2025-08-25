#pragma once

#include "warren/json/node.h"

namespace json {
namespace ast {

struct Boolean : public Node {
  explicit Boolean(bool value) : value(value) {}

  bool value;
};

}  // namespace ast
}  // namespace json
