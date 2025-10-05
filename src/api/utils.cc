#include "warren/json/utils.h"

#include <string>

namespace warren {
namespace json {

std::string to_string(const Value& value, const PrintOptions& opts) {
  struct Context {
    size_t level = 0;
    const PrintOptions& opts;

    std::string indent() { return std::string(level * opts.tab_width, ' '); }

    std::string print(const Value& value) {
      return value.visit(
          []() -> std::string { return "null"; },
          [](bool b) -> std::string { return (b ? "true" : "false"); },
          [](int32_t i) -> std::string { return std::to_string(i); },
          [](double d) -> std::string { return std::to_string(d); },
          [](const std::string& s) -> std::string { return "\"" + s + "\""; },
          [this](const array_t& a) -> std::string {
            std::string array;
            array += "[\n";
            level++;
            for (size_t i = 0; i < a.size(); i++) {
              array +=
                  indent() + print(a[i]) + (i < a.size() - 1 ? "," : "") + "\n";
            }

            level--;
            array += indent() + "]";

            return array;
          },
          [this](const object_t& o) -> std::string {
            std::string object;
            object += "{\n";
            level++;
            size_t i = 0;
            for (const auto& [k, v] : o) {
              object += indent() + "\"" + k + "\": " + print(v) +
                        (i++ < o.size() - 1 ? "," : "") + "\n";
            }

            level--;
            object += indent() + "}";

            return object;
          });
    }
  };

  return Context{.opts = opts}.print(value);
}

}  // namespace json
}  // namespace warren
