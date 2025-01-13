#include "visitors/set_visitor.h"

#include <string>

#include "json/value.h"
#include "nodes/array.h"
#include "nodes/boolean.h"
#include "nodes/node.h"
#include "nodes/null.h"
#include "nodes/number.h"
#include "nodes/object.h"
#include "nodes/string.h"

namespace json {

namespace visitors {

SetVisitor::SetVisitor(Node* target) : target_(target) {}

void SetVisitor::visit(Array& value) {
  target_->~Node();
  new (target_) Array(value.get());
}

void SetVisitor::visit(Boolean& value) {
  target_->~Node();
  new (target_) Boolean(value.get());
}

void SetVisitor::visit(Null& value) {
  target_->~Node();
  new (target_) Null();
}

void SetVisitor::visit(Number& value) {
  target_->~Node();
  new (target_) Number(value.get());
}

void SetVisitor::visit(Object& value) {
  target_->~Node();
  new (target_) Object(value.get());
}

void SetVisitor::visit(String& value) {
  target_->~Node();
  new (target_) String(value.get());
}

}  // namespace visitors

}  // namespace json
