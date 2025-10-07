#pragma once

#include <string_view>

#include "warren/json/trace/span.h"

namespace warren {
namespace trace {

struct NullSpan : public Span {
  void annotate(std::string_view, std::string_view) override {}
  void event(std::string_view) override {}
  void end() override {}
};

}  // namespace trace
}  // namespace warren
