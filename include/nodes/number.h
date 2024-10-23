#pragma once

#include "node.h"
#include "type.h"

namespace json {

class Number : public Node {
 public:
  void accept(Visitor& visitor) const override;
  Node* accept(ReturnVisitor& visitor) const override;

 public:
  Number(const double value);

 public:
  const double get() const;

 public:
  const Type type() const override;

 private:
  double value_;
};

}  // namespace json
