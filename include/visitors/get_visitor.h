#pragma once

#include <string>

#include "ret_visitor.h"
#include "types/array.h"
#include "types/boolean.h"
#include "types/null.h"
#include "types/number.h"
#include "types/object.h"
#include "types/string.h"
#include "types/type.h"
#include "utils/queue.h"
#include "visitor.h"

namespace json {

namespace visitors {

class GetVisitor : public ReturnVisitor {
 public:
  GetVisitor(Type* root, const utils::Queue<std::string>& keys);
  GetVisitor(const Type* root, const utils::Queue<std::string>& keys);
  Type* visit(const Array& node) override;
  Type* visit(const Boolean& node) override;
  Type* visit(const Null& node) override;
  Type* visit(const Number& node) override;
  Type* visit(const Object& node) override;
  Type* visit(const String& node) override;

 private:
  Type* ast_;
  utils::Queue<std::string> keys_;
};

}  // namespace visitors

}  // namespace json
