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
      const_cast<K&>(first) = other.first;
      const_cast<V&>(second) = other.second;
    }

    return *this;
  }

  Pair& operator=(Pair&& other) noexcept {
    if (this != &other) {
      const_cast<K&>(first) = std::move(other.first);
      const_cast<V&>(second) = std::move(other.second);
    }

    return *this;
  }

  bool operator==(const Pair& other) const {
    return first == other.first && second == other.second;
  }

  bool operator!=(const Pair& other) const { return !(*this == other); }
};

template <typename K, typename V>
bool operator==(const Pair<K, V>& lhs, const Pair<const K, const V>& rhs) {
  return lhs.first == rhs.first && lhs.second == rhs.second;
}

template <typename K, typename V>
bool operator==(const Pair<const K, const V>& lhs, const Pair<K, V>& rhs) {
  return operator==(rhs, lhs);
}

}  // namespace utils

}  // namespace json
