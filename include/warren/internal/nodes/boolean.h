#pragma once

#include "node.h"

namespace json {

namespace nodes {

class Boolean : public Node {
 public:
  Node* clone() const override;

 public:
  Boolean(const bool value);

 public:
  bool get();
  const bool get() const;

 private:
  bool value_;
};

}  // namespace nodes

}  // namespace json
