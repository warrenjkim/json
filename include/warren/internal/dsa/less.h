#pragma once

namespace json {

namespace dsa {

template <typename T>
struct less {
  constexpr bool operator()(const T& lhs, const T& rhs) const {
    return lhs < rhs;
  }
};

}  // namespace dsa

}  // namespace json
