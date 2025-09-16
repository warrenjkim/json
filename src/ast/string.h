#pragma once

#include <string>
#include <utility>

#include "warren/json/internal/ast/node.h"
#include "warren/json/internal/ast/visitor.h"

namespace json {
namespace ast {

struct String : public Node {
  void accept(Visitor& visitor) const override { visitor.visit(*this); }

  explicit String(std::string value) : value(std::move(value)) {}

  std::string value;
};

}  // namespace ast
}  // namespace json
