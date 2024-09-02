#pragma once

#include <cstdint>
#include <variant>

#include "node.h"

namespace json {

class JsonVisitor;

class NumberNode : public Node {
 public:
  void accept(JsonVisitor& visitor) const override;
  bool operator==(const Node& other) const override;

 public:
  NumberNode(const int64_t value);
  NumberNode(const double value);

 public:
  const std::variant<int64_t, double> get() const;

 private:
  std::variant<int64_t, double> value_;

 public:
  NumberNode() = delete;
  ~NumberNode() = default;
};

}  // namespace json
