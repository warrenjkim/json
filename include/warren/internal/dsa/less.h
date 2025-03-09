#pragma once

namespace json {

namespace utils {

template <typename T>
struct less {
  constexpr bool operator()(const T& lhs, const T& rhs) const {
    return lhs < rhs;
  }
};

}  // namespace utils

}  // namespace json
