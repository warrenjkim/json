#pragma once

#include <cstddef>
#include <string>

#include "type.h"
#include "utils/rbt.h"

namespace json {

class Object : public Type {
 public:
  ~Object();

 public:
  void accept(Visitor& visitor) const override;
  Type* accept(ReturnVisitor& visitor) const override;

 public:
  void add(const std::string& key, Type* value);
  const size_t size() const;
  const bool empty() const;

 public:
  utils::Map<std::string, Type*>& get();
  const utils::Map<std::string, Type*>& get() const;

 private:
  utils::Map<std::string, Type*> properties_;

 public:
  Object() = default;
};

}  // namespace json
