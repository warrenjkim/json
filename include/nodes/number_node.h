#pragma once

#include "node.h"

namespace json {

class NumberNode : public Node {
 public:
  void accept(Visitor& visitor) const override;

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
