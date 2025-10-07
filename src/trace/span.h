#pragma once

#include <string_view>

namespace warren {
namespace trace {

struct Span {
  virtual ~Span() = default;
  virtual void annotate(std::string_view key, std::string_view value) = 0;
  virtual void event(std::string_view name) = 0;
  virtual void end() = 0;
};

}  // namespace trace
}  // namespace warren
