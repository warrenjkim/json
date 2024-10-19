#pragma once

namespace json {

class Node;
class ArrayNode;
class BooleanNode;
class NullNode;
class NumberNode;
class ObjectNode;
class StringNode;

class Visitor {
 public:
  virtual void visit(const ArrayNode& node) = 0;
  virtual void visit(const BooleanNode& node) = 0;
  virtual void visit(const NullNode& node) = 0;
  virtual void visit(const NumberNode& node) = 0;
  virtual void visit(const ObjectNode& node) = 0;
  virtual void visit(const StringNode& node) = 0;
};

}  // namespace json
