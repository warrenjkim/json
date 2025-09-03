#pragma once

#include <string>
#include <utility>

#include "warren/json/node.h"
#include "warren/json/visitor.h"

namespace json {
namespace ast {

struct String : public Node {
  void accept(Visitor& visitor) override { visitor.visit(this); }

  explicit String(std::string value) : value(std::move(value)) {}

  std::string value;
};

}  // namespace ast
}  // namespace json
