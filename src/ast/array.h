#pragma once

#include <utility>
#include <vector>

#include "warren/json/node.h"

namespace json {
namespace ast {

struct Array : public Node {
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
