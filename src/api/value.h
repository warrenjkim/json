#pragma once

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

 private:
  enum Type { ARRAY, BOOLEAN, JSON_NULL, NUMBER, OBJECT, STRING } type_;
  union {
    array_t a;
    bool b;
    double n;
    object_t o;
    std::string s;
  };
};

}  // namespace json
