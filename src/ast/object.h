#pragma once

#include <map>
#include <string>
#include <utility>

#include "warren/json/node.h"

namespace json {
namespace ast {

struct Object : public Node {
  explicit Object(std::map<std::string, Node*> value)
      : value(std::move(value)) {}

  ~Object() {
    for (auto [_, node] : value) {
      delete node;
    }
  }

  std::map<std::string, Node*> value;
};

}  // namespace ast
}  // namespace json
