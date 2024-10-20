#pragma once

namespace json {

class Type;
class Array;
class Boolean;
class Null;
class Number;
class Object;
class String;

class Visitor {
 public:
  virtual void visit(const Array& node) = 0;
  virtual void visit(const Boolean& node) = 0;
  virtual void visit(const Null& node) = 0;
  virtual void visit(const Number& node) = 0;
  virtual void visit(const Object& node) = 0;
  virtual void visit(const String& node) = 0;

 public:
  ~Visitor() = default;
};

}  // namespace json
