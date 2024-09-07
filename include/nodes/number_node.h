#pragma once

#include "node.h"

namespace json {

class JsonVisitor;

class NumberNode : public Node {
 public:
  void accept(JsonVisitor& visitor) const override;
  bool operator==(const Node& other) const override;

 public:
  NumberNode(const double value);

 public:
  const double get() const;

 private:
  double value_;

 public:
  NumberNode() = delete;
  ~NumberNode() = default;
};

}  // namespace json
