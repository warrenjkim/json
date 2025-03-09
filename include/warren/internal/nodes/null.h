#pragma once

#include <cstddef>

#include "node.h"

namespace json {

namespace nodes {

class Null : public Node {
 public:
  void accept(visitors::Visitor& visitor) override;
  void accept(visitors::ConstVisitor& visitor) const override;

 public:
  Node* clone() override;

 public:
  operator nullptr_t() const;
};

}  // namespace nodes

}  // namespace json
