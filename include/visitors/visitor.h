#pragma once

namespace json {

class Node;
class Array;
class Boolean;
class Null;
class Number;
class Object;
class String;

namespace visitors {

class ConstVisitor {
 public:
  virtual void visit(const Array& node) = 0;
  virtual void visit(const Boolean& node) = 0;
  virtual void visit(const Null& node) = 0;
  virtual void visit(const Number& node) = 0;
  virtual void visit(const Object& node) = 0;
  virtual void visit(const String& node) = 0;

 public:
  ~ConstVisitor() = default;
};

class Visitor {
 public:
  virtual void visit(Array& node) = 0;
  virtual void visit(Boolean& node) = 0;
  virtual void visit(Null& node) = 0;
  virtual void visit(Number& node) = 0;
  virtual void visit(Object& node) = 0;
  virtual void visit(String& node) = 0;

 public:
  ~Visitor() = default;
};

}  // namespace visitors

}  // namespace json
