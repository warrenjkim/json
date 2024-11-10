#pragma once

#include "node.h"

namespace json {

class Number : public Node {
 public:
  void accept(visitors::Visitor& visitor) override;
  void accept(visitors::ConstVisitor& visitor) const override;

 public:
  Number(const double value);

 public:
  double get();
  const double get() const;

 public:
  double* ptr();
  const double* ptr() const;

 private:
  double value_;
};

}  // namespace json
