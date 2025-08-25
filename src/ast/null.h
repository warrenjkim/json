#pragma once

#include <cstddef>

#include "warren/json/node.h"

namespace json {
namespace ast {

struct Null : public Node {
  explicit Null() : value(nullptr) {}

  nullptr_t value;
};

}  // namespace ast
}  // namespace json
