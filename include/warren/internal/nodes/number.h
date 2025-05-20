#pragma once

#include "node.h"
#include "warren/internal/dsa/numeric.h"
#include "warren/internal/utils/concepts.h"

namespace json {

namespace nodes {

class Number : public Node {
 public:
  void accept(visitors::Visitor& visitor) override;
  void accept(visitors::ConstVisitor& visitor) const override;

 public:
  Node* clone() const override;

 public:
  Number(const dsa::Numeric& value);

  template <ReasonableNumber T>
  Number(const T value) : value_(dsa::Numeric(value)) {}

 public:
  dsa::Numeric get();
  const dsa::Numeric get() const;

 private:
  dsa::Numeric value_;
};

}  // namespace nodes

}  // namespace json
