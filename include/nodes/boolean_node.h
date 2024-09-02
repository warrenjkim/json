#pragma once

#include "node.h"

namespace json {

class JsonVisitor;

class BooleanNode : public Node {
 public:
  void accept(JsonVisitor& visitor) const override;

 public:
  BooleanNode(const bool value);
  bool operator==(const Node& other) const override;

 public:
  const bool get() const;

 private:
  bool value_;

 public:
  BooleanNode() = delete;
  ~BooleanNode() = default;
};

}  // namespace json
