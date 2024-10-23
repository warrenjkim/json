#pragma once

#include <string>

#include "nodes/array.h"
#include "nodes/boolean.h"
#include "nodes/node.h"
#include "nodes/null.h"
#include "nodes/number.h"
#include "nodes/object.h"
#include "nodes/string.h"
#include "ret_visitor.h"
#include "utils/queue.h"
#include "visitor.h"

namespace json {

namespace visitors {

class GetVisitor : public ReturnVisitor {
 public:
  GetVisitor(Node* root, const utils::Queue<std::string>& keys);
  GetVisitor(const Node* root, const utils::Queue<std::string>& keys);
  Node* visit(const Array& node) override;
  Node* visit(const Boolean& node) override;
  Node* visit(const Null& node) override;
  Node* visit(const Number& node) override;
  Node* visit(const Object& node) override;
  Node* visit(const String& node) override;

 private:
  Node* ast_;
  utils::Queue<std::string> keys_;
};

}  // namespace visitors

}  // namespace json
