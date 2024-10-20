#pragma once

namespace json {

class Type;
class Array;
class Boolean;
class Null;
class Number;
class Object;
class String;

class ReturnVisitor {
 public:
  virtual Type* visit(const Array& node) = 0;
  virtual Type* visit(const Boolean& node) = 0;
  virtual Type* visit(const Null& node) = 0;
  virtual Type* visit(const Number& node) = 0;
  virtual Type* visit(const Object& node) = 0;
  virtual Type* visit(const String& node) = 0;

 public:
  ~ReturnVisitor() = default;
};

}  // namespace json
