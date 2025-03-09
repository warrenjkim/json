#pragma once

namespace json {

namespace dsa {

template <typename T>
struct key {
  const T& operator()(const T& data) const { return data; }
};

}  // namespace dsa

}  // namespace json
