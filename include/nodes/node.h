#pragma once

namespace json {

class Visitor;

class Node {
 public:
  virtual void accept(Visitor& visitor) const = 0;

 public:
  const bool operator==(const Node& other) const;
  const bool operator!=(const Node& other) const;

 public:
  virtual ~Node() noexcept = default;
};

class ArrayNode;
class BooleanNode;
class NullNode;
class NumberNode;
class ObjectNode;
class KeyValueNode;
class StringNode;

}  // namespace json
