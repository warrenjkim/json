#pragma once

#include "node.h"

namespace json {

class Boolean : public Node {
 public:
  void accept(visitors::Visitor& visitor) const override;
  Node* accept(visitors::ReturnVisitor& visitor) const override;

 public:
  Boolean(const bool value);

 public:
  bool& get();
  const bool& get() const;

 private:
  bool value_;
};

}  // namespace json
