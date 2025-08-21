#pragma once

#include <cstddef>

#include "node.h"

namespace json {

namespace nodes {

class Null : public Node {
 public:
  Node* clone() const override;

 public:
  operator nullptr_t() const;
};

}  // namespace nodes

}  // namespace json
