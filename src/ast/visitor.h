#pragma once

#include "node.h"

namespace json {
namespace ast {

struct Node;

class Visitor {
 public:
  virtual void visit(Node* node) = 0;
};

}  // namespace ast
}  // namespace json
