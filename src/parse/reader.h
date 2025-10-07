#pragma once

#include <cstddef>
#include <optional>
#include <string>

namespace warren {
namespace json {

struct Reader {
 public:
  inline explicit Reader(std::string json) : json_(std::move(json)), pos_(0) {}

  inline bool eof() const { return pos_ >= json_.length(); }

  inline size_t tell() const { return pos_; }

  inline char peek() const { return json_[pos_]; }

  inline char get() { return json_[pos_++]; }

  inline bool expect(char c) { return json_[pos_] == c && pos_++; }

  inline std::string substr(size_t start,
                            std::optional<size_t> length = std::nullopt) const {
    if (length) {
      return json_.substr(start, *length);
    }

    return json_.substr(start);
  }

 private:
  std::string json_;
  size_t pos_;
};

}  // namespace json
}  // namespace warren
