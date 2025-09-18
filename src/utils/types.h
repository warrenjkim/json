#pragma once

#include <map>
#include <vector>

namespace json {

class Value;

using array_t = std::vector<json::Value>;
using object_t = std::map<std::string, json::Value>;

}  // namespace json
