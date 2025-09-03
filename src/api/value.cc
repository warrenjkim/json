#include "value.h"

#include <cstddef>
#include <cstdint>
#include <new>

namespace json {

Value::Value() : type_(Type::JSON_NULL) {}

Value::~Value() {
  switch (type_) {
    case Type::ARRAY:
      a_.~vector();
      break;
    case Type::OBJECT:
      o_.~map();
      break;
    case Type::STRING:
      s_.~basic_string();
      break;
    default:
      break;
  }

  type_ = Type::JSON_NULL;
}

Value::Value(bool b) : b_(b), type_(Type::BOOLEAN) {}

Value::Value(double n) : n_(n), type_(Type::DOUBLE) {}

Value::Value(int64_t n) : n_((double)n), type_(Type::INTEGRAL) {}

Value::Value(nullptr_t) : type_(Type::JSON_NULL) {}

Value::Value(const char* s) {
  ::new (&s_) std::string(s);
  type_ = Type::STRING;
}

Value::Value(std::string s) {
  ::new (&s_) std::string(std::move(s));
  type_ = Type::STRING;
}

Value::Value(array_t a) {
  ::new (&a_) array_t(std::move(a));
  type_ = Type::ARRAY;
}

Value::Value(object_t o) {
  ::new (&o_) object_t(std::move(o));
  type_ = Type::OBJECT;
}

}  // namespace json
