#pragma once

namespace json {

class Visitor;

class Type {
 public:
  virtual void accept(Visitor& visitor) const = 0;

 public:
  const bool operator==(const Type& other) const;
  const bool operator!=(const Type& other) const;

 public:
  virtual ~Type() noexcept = default;
};

class Array;
class Boolean;
class Null;
class Number;
class Object;
class KeyValue;
class String;

}  // namespace json
