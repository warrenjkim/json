#pragma once

#include <chrono>
#include <string>

namespace warren {
namespace trace {

struct Event {
  std::string name;
  std::chrono::steady_clock::time_point relative_timestamp;
};

}  // namespace trace
}  // namespace warren
