#include "value.h"

namespace json {

Value::Value() : type_(Type::JSON_NULL) {}

Value::~Value() {
  switch (type_) {
    case Type::ARRAY:
      a.~vector();
      break;
    case Type::OBJECT:
      o.~map();
      break;
    case Type::STRING:
      s.~basic_string();
      break;
    default:
      break;
  }

  type_ = Type::JSON_NULL;
}

}  // namespace json
