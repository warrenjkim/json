#pragma once

#include "type.h"

namespace json {

class Number : public Type {
 public:
  void accept(Visitor& visitor) const override;

 public:
  Number(const double value);

 public:
  const double get() const;

 private:
  double value_;

 public:
  Number() = delete;
  ~Number() = default;
};

}  // namespace json
