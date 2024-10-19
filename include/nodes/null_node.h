#pragma once

#include "node.h"

namespace json {

class NullNode : public Node {
 public:
  void accept(Visitor& visitor) const override;

 public:
  NullNode() = default;
  ~NullNode() = default;
};

}  // namespace json
