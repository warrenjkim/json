#pragma once

#include <cstddef>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace json {

class Value;

using array_t = std::vector<json::Value>;
using object_t = std::map<std::string, json::Value>;

class Value {
 public:
  Value();
  ~Value();

  Value(bool b);
  Value(double n);
  Value(int64_t n);
  Value(nullptr_t);
  Value(const char* s);
  Value(std::string s);

  explicit Value(array_t a);
  explicit Value(object_t o);

 private:
  union {
    array_t a_;
    bool b_;
    double n_;
    object_t o_;
    std::string s_;
  };

  enum Type {
    ARRAY,
    BOOLEAN,
    JSON_NULL,
    INTEGRAL,
    DOUBLE,
    OBJECT,
    STRING
  } type_;
};

}  // namespace json
