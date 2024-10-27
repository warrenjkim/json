#pragma once

namespace json {

namespace visitors {

class Visitor;
class ConstVisitor;

}  // namespace visitors

class Node {
 public:
  virtual void accept(visitors::Visitor& visitor) = 0;
  virtual void accept(visitors::ConstVisitor& visitor) const = 0;

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
