#pragma once

#include <optional>
#include <stack>
#include <string_view>
#include <vector>

#include "warren/trace/attribute.h"
#include "warren/trace/event.h"
#include "warren/trace/tracer.h"

namespace warren {
namespace trace {

class InMemoryTracer final : public Tracer {
 public:
  virtual Span* make_span(std::string_view name) override;

 private:
  class InMemorySpan final : public Span {
   public:
    InMemorySpan(size_t id, InMemoryTracer& tracer)
        : id_(id), tracer_(tracer) {}

    inline virtual void annotate(std::string_view key,
                                 std::string_view value) override {
      tracer_.annotate_callback(id_, key, value);
    }

    inline virtual void event(std::string_view event) override {
      tracer_.event_callback(id_, event);
    }

    inline virtual void end() override { tracer_.end_callback(id_); }

   private:
    size_t id_;
    InMemoryTracer& tracer_;
  };

  struct SpanData {
    // size_t id; TODO(make this a uuid)
    std::string name;
    std::chrono::steady_clock::time_point start;
    std::chrono::steady_clock::time_point end;
    std::vector<Attribute> attributes;
    std::vector<Event> events;

    std::optional<size_t> parent;
    std::vector<size_t> children;
  };

  void annotate_callback(size_t id, std::string_view key,
                         std::string_view value);

  void event_callback(size_t id, std::string_view event);

  void end_callback(size_t id);

  std::vector<SpanData> spans_;
};

}  // namespace trace
}  // namespace warren
