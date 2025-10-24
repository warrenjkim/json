#include "warren/trace/in_memory_tracer.h"

#include <string_view>

#include "warren/trace/tracer.h"

namespace warren {
namespace trace {

Span* InMemoryTracer::make_span(std::string_view name) {
  spans_.push_back(SpanData{.name = std::string(name)});
  return new InMemorySpan(/*id=*/spans_.size() - 1, *this);
}

void InMemoryTracer::annotate_callback(size_t id, std::string_view key,
                                       std::string_view value) {
  spans_[id].attributes.push_back(
      Attribute{.key = std::string(key), .value = std::string(value)});
}

void InMemoryTracer::end_callback(size_t id) {
  for (size_t child : spans_[id].children) {
    end_callback(child);
  }
}

}  // namespace trace
}  // namespace warren
