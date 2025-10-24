#pragma once

#include <string_view>

#include "warren/trace/tracer.h"

namespace warren {
namespace trace {

class NullTracer : public Tracer {
 public:
  explicit NullTracer() : span_(new NullSpan()) {}

  ~NullTracer() { delete span_; }

  Span* make_span(std::string_view) override { return span_; }

 private:
  class NullSpan : public Span {
    void annotate(std::string_view, std::string_view) override {}
    void event(std::string_view) override {}
    void end() override {}
  };

  NullSpan* const span_;
};

}  // namespace trace
}  // namespace warren
