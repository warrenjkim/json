#pragma once

#include "type.h"

namespace json {

class Boolean : public Type {
 public:
  void accept(Visitor& visitor) const override;
  Type* accept(ReturnVisitor& visitor) const override;

 public:
  Boolean(const bool value);

 public:
  const bool get() const;

 private:
  bool value_;

 public:
  Boolean() = delete;
  ~Boolean() = default;
};

}  // namespace json
