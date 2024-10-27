#pragma once

#include "node.h"

namespace json {

class Null : public Node {
 public:
  void accept(visitors::Visitor& visitor) const override;
  Node* accept(visitors::ReturnVisitor& visitor) const override;
};

}  // namespace json
