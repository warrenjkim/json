#pragma once

#include "types/array.h"
#include "types/boolean.h"
#include "types/null.h"
#include "types/number.h"
#include "types/object.h"
#include "types/string.h"
#include "types/type.h"
#include "visitor.h"

namespace json {

namespace visitors {

class CmpVisitor : public Visitor {
 public:
  CmpVisitor(Type* root);
  CmpVisitor(const Type* root);
  void visit(const Array& node) override;
  void visit(const Boolean& node) override;
  void visit(const Null& node) override;
  void visit(const Number& node) override;
  void visit(const Object& node) override;
  void visit(const String& node) override;
  bool result() const;

 private:
  bool result_;
  Type* expected_;
};

}  // namespace visitors

}  // namespace json
