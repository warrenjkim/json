#pragma once

#include "node.h"

namespace json {
namespace ast {

class Visitor {
 public:
  virtual void visit(Node* node) = 0;
};

}  // namespace ast
}  // namespace json
