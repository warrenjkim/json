#pragma once

#include <string>

#include "node.h"
#include "type.h"

namespace json {

class String : public Node {
 public:
  void accept(Visitor& visitor) const override;
  Node* accept(ReturnVisitor& visitor) const override;

 public:
  String(std::string value);

 public:
  const std::string& get() const;

 public:
  const Type type() const override;

 private:
  std::string value_;

 public:
  String() = delete;
};

}  // namespace json
