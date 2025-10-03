#pragma once

#include <map>
#include <vector>

namespace warren {
namespace json {

class Value;

using array_t = std::vector<Value>;
using object_t = std::map<std::string, Value>;

}  // namespace json
}  // namespace warren
