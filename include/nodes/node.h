#pragma once

namespace json {

class JsonVisitor;

class Node {
 public:
  virtual void accept(JsonVisitor& visitor) const = 0;

 public:
  virtual bool operator==(const Node& other) const = 0;
  bool operator!=(const Node& other) const = default;

 public:
  virtual ~Node() noexcept = default;
};

}  // namespace json
