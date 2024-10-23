#pragma once

#include "node.h"
#include "type.h"

namespace json {

class Null : public Node {
 public:
  void accept(Visitor& visitor) const override;
  Node* accept(ReturnVisitor& visitor) const override;

 public:
  const Type type() const override;
};

}  // namespace json
