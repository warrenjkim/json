#pragma once

#include "node.h"

namespace json {

class Null : public Node {
 public:
  void accept(visitors::Visitor& visitor) override;
  void accept(visitors::ConstVisitor& visitor) const override;
};

}  // namespace json
