#pragma once

#include <utility>
#include <vector>

#include "node.h"
#include "visitor.h"

namespace json {
namespace ast {

struct Array : public Node {
  void accept(Visitor& visitor) const override { visitor.visit(*this); }

  explicit Array(std::vector<Node*> value) : value(std::move(value)) {}

  ~Array() {
    for (Node* node : value) {
      delete node;
    }
  }

  std::vector<Node*> value;
};

}  // namespace ast
}  // namespace json
