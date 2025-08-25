#pragma once

namespace json {
namespace ast {

struct Node {
  virtual ~Node() noexcept = default;
};

}  // namespace ast
}  // namespace json
