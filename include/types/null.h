#pragma once

#include "type.h"

namespace json {

class Null : public Type {
 public:
  void accept(Visitor& visitor) const override;
  Type* accept(ReturnVisitor& visitor) const override;

 public:
  Null() = default;
  ~Null() = default;
};

}  // namespace json
