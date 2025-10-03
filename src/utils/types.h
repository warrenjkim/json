#pragma once

#include <map>
#include <vector>

namespace json {

class Value;

using array_t = std::vector<Value>;
using object_t = std::map<std::string, Value>;

}  // namespace json
