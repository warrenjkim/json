#pragma once

#include <string_view>

namespace warren {
namespace trace {

struct Span;

struct Tracer {
  virtual ~Tracer() = default;
  virtual Span* make_span(std::string_view name) = 0;
};

}  // namespace trace
}  // namespace warren
