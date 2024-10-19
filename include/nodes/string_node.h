#pragma once

#include <string>

#include "node.h"

namespace json {

class StringNode : public Node {
 public:
  void accept(Visitor& visitor) const override;

 public:
  StringNode(std::string value);

 public:
  const std::string& get() const;

 private:
  std::string value_;

 public:
  StringNode() = delete;
  ~StringNode() = default;
};

}  // namespace json
