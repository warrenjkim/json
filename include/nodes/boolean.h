#pragma once

#include "node.h"

namespace json {

class Boolean : public Node {
 public:
  void accept(visitors::Visitor& visitor) override;
  void accept(visitors::ConstVisitor& visitor) const override;

 public:
  Boolean(const bool value);

 public:
  bool get();
  const bool get() const;

  bool* ptr();
  const bool* ptr() const;

 private:
  bool value_;
};

}  // namespace json
