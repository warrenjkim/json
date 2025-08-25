#pragma once

#include <string>
#include <utility>

#include "warren/json/node.h"

namespace json {
namespace ast {

struct String : public Node {
  explicit String(std::string value) : value(std::move(value)) {}

  std::string value;
};

}  // namespace ast
}  // namespace json
