#pragma once

#include "node.h"

namespace json {

class Number : public Node {
 public:
  void accept(visitors::Visitor& visitor) const override;
  Node* accept(visitors::ReturnVisitor& visitor) const override;

 public:
  Number(const double value);

 public:
  double& get();
  const double& get() const;

 private:
  double value_;
};

}  // namespace json
