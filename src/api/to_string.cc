#include "warren/json/to_string.h"

#include <iomanip>
#include <sstream>
#include <string>

#include "warren/json/utils/types.h"

namespace {

struct Printer {
  size_t level = 0;
  const warren::json::PrintOptions& opts;

  std::string indent() {
    return opts.compact ? "" : std::string(level * opts.tab_width, ' ');
  }

  std::string format(double d) {
    std::ostringstream oss;
    oss << std::setprecision(15) << std::defaultfloat << d;

    return oss.str();
  }

  std::string print(const warren::json::Value& value) {
    return value.visit(
        []() -> std::string { return "null"; },
        [](bool b) -> std::string { return (b ? "true" : "false"); },
        [](int32_t i) -> std::string { return std::to_string(i); },
        [this](double d) -> std::string { return format(d); },
        [](const std::string& s) -> std::string { return "\"" + s + "\""; },
        [this](const warren::json::array_t& a) -> std::string {
          if (a.empty()) {
            return "[]";
          }

          std::string array;
          array += "[";
          array += (opts.compact ? "" : "\n");
          level++;
          for (size_t i = 0; i < a.size(); i++) {
            array += indent() + print(a[i]) +
                     (i < a.size() - 1 || opts.trailing_commas ? "," : "") +
                     (opts.compact ? "" : "\n");
          }

          level--;
          array += indent() + "]";

          return array;
        },
        [this](const warren::json::object_t& o) -> std::string {
          if (o.empty()) {
            return "{}";
          }

          std::string object;
          object += "{";
          object += (opts.compact ? "" : "\n");
          level++;
          size_t i = 0;
          for (const auto& [k, v] : o) {
            object += indent() + "\"" + k + "\":" + (opts.compact ? "" : " ") +
                      print(v) +
                      (i++ < o.size() - 1 || opts.trailing_commas ? "," : "") +
                      (opts.compact ? "" : "\n");
          }

          level--;
          object += indent() + "}";

          return object;
        });
  }
};

}  // namespace

namespace warren {
namespace json {

std::string to_string(const Value& value, const PrintOptions& opts) {
  return Printer{.opts = opts}.print(value);
}

}  // namespace json
}  // namespace warren
