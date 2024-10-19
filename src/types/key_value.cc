#include "types/key_value.h"

#include <utility>  // move

#include "types/type.h"

namespace json {

void KeyValue::accept(Visitor& visitor) const { return; }

KeyValue::KeyValue(std::string key, Type* value)
    : key_(std::move(key)), value_(value) {}

const std::string& KeyValue::key() const { return key_; }

Type* KeyValue::value() { return value_; }

}  // namespace json
