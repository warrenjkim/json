#pragma once

namespace json {

class JsonNode;
class ArrayNode;
class BooleanNode;
class NullNode;
class NumberNode;
class ObjectNode;
class KeyValueNode;
class StringNode;

class JsonVisitor {
 public:
  virtual void visit(const ArrayNode& node) = 0;
  virtual void visit(const BooleanNode& node) = 0;
  virtual void visit(const NullNode& node) = 0;
  virtual void visit(const NumberNode& node) = 0;
  virtual void visit(const ObjectNode& node) = 0;
  virtual void visit(const KeyValueNode& node) = 0;
  virtual void visit(const StringNode& node) = 0;
};

}  // namespace json
