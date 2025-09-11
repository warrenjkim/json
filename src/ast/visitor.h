#pragma once

namespace json {
namespace ast {

struct Array;
struct Boolean;
struct Null;
struct Number;
struct Object;
struct String;

class Visitor {
 public:
  virtual ~Visitor() = default;

  virtual void visit(const Array& node) = 0;
  virtual void visit(const Boolean& node) = 0;
  virtual void visit(const Null& node) = 0;
  virtual void visit(const Number& node) = 0;
  virtual void visit(const Object& node) = 0;
  virtual void visit(const String& node) = 0;
};

}  // namespace ast
}  // namespace json
