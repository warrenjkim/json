#pragma once

namespace json {

namespace nodes {

class Node;
class Array;
class Boolean;
class Null;
class Number;
class Object;
class String;

}  // namespace nodes

namespace visitors {

class ConstVisitor {
 public:
  virtual void visit(const nodes::Array& node) = 0;
  virtual void visit(const nodes::Boolean& node) = 0;
  virtual void visit(const nodes::Null& node) = 0;
  virtual void visit(const nodes::Number& node) = 0;
  virtual void visit(const nodes::Object& node) = 0;
  virtual void visit(const nodes::String& node) = 0;

 public:
  ~ConstVisitor() = default;
};

class Visitor {
 public:
  virtual void visit(nodes::Array& node) = 0;
  virtual void visit(nodes::Boolean& node) = 0;
  virtual void visit(nodes::Null& node) = 0;
  virtual void visit(nodes::Number& node) = 0;
  virtual void visit(nodes::Object& node) = 0;
  virtual void visit(nodes::String& node) = 0;

 public:
  ~Visitor() = default;
};

}  // namespace visitors

}  // namespace json
