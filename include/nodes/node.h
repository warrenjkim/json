#pragma once

namespace json {

class Visitor;
class ReturnVisitor;

class Node {
 public:
  virtual void accept(Visitor& visitor) const = 0;
  virtual Node* accept(ReturnVisitor& visitor) const = 0;

 public:
  template <typename T>
  T* as() {
    return dynamic_cast<T*>(this);
  }

  template <typename T>
  const T* as() const {
    return dynamic_cast<const T*>(this);
  }

 public:
  const bool operator==(const Node& other) const;
  const bool operator!=(const Node& other) const;

 public:
  virtual ~Node() noexcept = default;
};

class Array;
class Boolean;
class Null;
class Number;
class Object;
class String;

}  // namespace json
