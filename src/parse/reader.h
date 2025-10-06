#pragma once

#include <cstddef>
#include <string>

struct Reader {
 public:
  inline explicit Reader(std::string json) : json_(std::move(json)), pos_(0) {}

  inline bool eof() { return pos_ >= json_.length(); }

  inline size_t tell() { return pos_; }

  inline char peek() { return pos_ >= json_.length() ? '\0' : json_[pos_]; }

  inline char get() { return pos_ >= json_.length() ? '\0' : json_[pos_++]; }

  inline bool expect(char c) {
    return pos_ < json_.length() && json_[pos_] == c && pos_++;
  }

  inline std::string substr(size_t start, size_t length) {
    return json_.substr(start, length);
  }

 private:
  std::string json_;
  size_t pos_;
};
