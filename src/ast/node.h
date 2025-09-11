#pragma once

#include "visitor.h"

namespace json {
namespace ast {

class Visitor;

struct Node {
  virtual void accept(Visitor& visitor) const = 0;

  virtual ~Node() noexcept = default;
};

}  // namespace ast
}  // namespace json
