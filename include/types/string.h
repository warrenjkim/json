#pragma once

#include <string>

#include "type.h"

namespace json {

class String : public Type {
 public:
  void accept(Visitor& visitor) const override;

 public:
  String(std::string value);

 public:
  const std::string& get() const;

 private:
  std::string value_;

 public:
  String() = delete;
  ~String() = default;
};

}  // namespace json
