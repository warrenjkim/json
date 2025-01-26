#pragma once

// TODO(for each impl, it might be useful to add an implicit conversion [i.e.
// operator T()].)

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
  virtual Node* clone() = 0;

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
