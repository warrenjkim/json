#pragma once

#include "node.h"
#include "visitors/visitor.h"

namespace json {

class JsonVisitor;

class NullNode : public Node {
 public:
  void accept(JsonVisitor& visitor) const override;
  bool operator==(const Node& other) const override;

 public:
  NullNode() = default;
  ~NullNode() = default;
};

}  // namespace json
