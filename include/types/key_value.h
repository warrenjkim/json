#pragma once

#include <string>

#include "type.h"

namespace json {

class KeyValue : public Type {
 public:
  void accept(Visitor& visitor) const override;

 public:
  KeyValue(std::string key, Type* value);

 public:
  const std::string& key() const;
  Type* value();

 private:
  std::string key_;
  Type* value_;

 public:
  KeyValue() = delete;
  ~KeyValue() = default;
};

}  // namespace json
