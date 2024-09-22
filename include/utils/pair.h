#pragma once
#include <utility>  // move

namespace json {

namespace utils {

template <typename K, typename V>
struct Pair {
  K first;
  V second;

  Pair() : first(K()), second(V()) {}

  ~Pair() = default;

  Pair(const K& first, const V& second) : first(first), second(second) {}

  Pair(const Pair& other) : first(other.first), second(other.second) {}

  Pair(Pair&& other) noexcept
      : first(std::move(other.first)), second(std::move(other.second)) {}

  Pair& operator=(const Pair& other) {
    if (this != &other) {
      first = other.first;
      second = other.second;
    }

    return *this;
  }

  Pair& operator=(Pair&& other) noexcept {
    if (this != &other) {
      first = std::move(other.first);
      second = std::move(other.second);
    }

    return *this;
  }

  bool operator==(const Pair& other) const {
    return first == other.first && second == other.second;
  }

  bool operator!=(const Pair& other) const { return !(*this == other); }
};

}  // namespace utils

}  // namespace json
