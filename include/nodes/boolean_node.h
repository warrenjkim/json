#pragma once

#include "node.h"

namespace json {

class BooleanNode : public Node {
 public:
  void accept(Visitor& visitor) const override;

 public:
  BooleanNode(const bool value);

 public:
  const bool get() const;

 private:
  bool value_;

 public:
  BooleanNode() = delete;
  ~BooleanNode() = default;
};

}  // namespace json
