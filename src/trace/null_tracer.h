#pragma once

#include <string_view>
#include <vector>

#include "warren/json/trace/null_span.h"
#include "warren/json/trace/tracer.h"

namespace warren {
namespace trace {

struct Span;

class NullTracer : public Tracer {
 public:
  ~NullTracer() {
    for (Span* span : spans_) {
      delete span;
    }
  }

  Span* make_span(std::string_view) override {
    spans_.push_back(new NullSpan{});
    return spans_.back();
  }

 private:
  std::vector<Span*> spans_;
};

}  // namespace trace
}  // namespace warren
