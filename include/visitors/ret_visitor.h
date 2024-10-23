#pragma once

namespace json {

class Node;
class Array;
class Boolean;
class Null;
class Number;
class Object;
class String;

class ReturnVisitor {
 public:
  virtual Node* visit(const Array& node) = 0;
  virtual Node* visit(const Boolean& node) = 0;
  virtual Node* visit(const Null& node) = 0;
  virtual Node* visit(const Number& node) = 0;
  virtual Node* visit(const Object& node) = 0;
  virtual Node* visit(const String& node) = 0;

 public:
  ~ReturnVisitor() = default;
};

}  // namespace json
