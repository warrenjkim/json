#pragma once

namespace json {

namespace utils {

template <typename T>
struct key {
  const T& operator()(const T& data) const { return data; }
};

}  // namespace utils

}  // namespace json
