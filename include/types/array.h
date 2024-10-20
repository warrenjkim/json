#pragma once

#include <vector>

#include "type.h"

namespace json {

class Array : public Type {
 public:
  void accept(Visitor& visitor) const override;
  Type* accept(ReturnVisitor& visitor) const override;

 public:
  void add(Type* element);
  const size_t size() const;
  const bool empty() const;

 public:
  std::vector<Type*>& get();
  const std::vector<Type*>& get() const;

 private:
  std::vector<Type*> array_;

 public:
  Array() = default;
  ~Array() = default;
};

}  // namespace json
