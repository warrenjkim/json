#pragma once

#include "node.h"

namespace json {

namespace nodes {

class Number : public Node {
 public:
  void accept(visitors::Visitor& visitor) override;
  void accept(visitors::ConstVisitor& visitor) const override;

 public:
  Node* clone() override;

 public:
  Number(const double value);

 public:
  double get();
  const double get() const;

 private:
  double value_;
};

}  // namespace nodes

}  // namespace json
