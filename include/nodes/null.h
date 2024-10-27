#pragma once

#include "node.h"

namespace json {

class Null : public Node {
 public:
  void accept(Visitor& visitor) const override;
  Node* accept(ReturnVisitor& visitor) const override;
};

}  // namespace json
