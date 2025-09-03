#include "value.h"

#include <cstddef>
#include <cstdint>
#include <new>

namespace json {

Value::Value() : type_(Type::JSON_NULL) {}

Value::~Value() { destroy(); }

Value::Value(bool b) : b_(b), type_(Type::BOOLEAN) {}

Value::Value(double n) : n_(n), type_(Type::DOUBLE) {}

Value::Value(int64_t n) : n_((double)n), type_(Type::INTEGRAL) {}

Value::Value(nullptr_t) : type_(Type::JSON_NULL) {}

Value::Value(const char* s) {
  ::new ((void*)(&s_)) std::string(s);
  type_ = Type::STRING;
}

Value::Value(std::string s) {
  ::new ((void*)(&s_)) std::string(std::move(s));
  type_ = Type::STRING;
}

Value& Value::operator=(const char* s) { return *this = std::string(s); }

Value& Value::operator=(std::string s) {
  destroy();
  ::new ((void*)(&s_)) std::string(std::move(s));
  type_ = Type::STRING;

  return *this;
}

Value::Value(array_t a) {
  ::new ((void*)(&a_)) array_t(std::move(a));
  type_ = Type::ARRAY;
}

Value& Value::operator=(array_t a) {
  destroy();
  ::new ((void*)(&a_)) array_t(std::move(a));
  type_ = Type::ARRAY;

  return *this;
}

Value::Value(object_t o) {
  ::new ((void*)(&o_)) object_t(std::move(o));
  type_ = Type::OBJECT;
}

Value& Value::operator=(object_t o) {
  destroy();
  ::new ((void*)(&o_)) object_t(std::move(o));
  type_ = Type::OBJECT;

  return *this;
}

}  // namespace json
