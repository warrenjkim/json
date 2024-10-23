#pragma once

#include "node.h"
#include "type.h"

namespace json {

class Boolean : public Node {
 public:
  void accept(Visitor& visitor) const override;
  Node* accept(ReturnVisitor& visitor) const override;

 public:
  Boolean(const bool value);

 public:
  const bool get() const;

 public:
  const Type type() const override;

 private:
  bool value_;
};

}  // namespace json
