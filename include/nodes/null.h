#pragma once

#include <cstddef>

#include "node.h"

namespace json {

class Null : public Node {
 public:
  void accept(visitors::Visitor& visitor) override;
  void accept(visitors::ConstVisitor& visitor) const override;

 public:
  operator std::nullptr_t() const;
};

}  // namespace json
