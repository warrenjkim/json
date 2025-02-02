#pragma once

#include <cstddef>  // size_t
#include <optional>

#include "less.h"
#include "pair.h"
#include "utils/rbt.h"

namespace json {

namespace utils {

template <typename K, typename V>
class Map {
 public:
  class Iterator;
  class ConstIterator;

 public:
  Map() = default;
  ~Map() = default;
  Map(const Map& other) = default;
  Map(Map&& other) noexcept = default;
  Map& operator=(const Map& other) = default;
  Map& operator=(Map&& other) noexcept = default;

 public:
  bool operator==(const Map& other) const;
  bool operator!=(const Map& other) const;

 public:
  constexpr size_t size() const;
  constexpr bool empty() const;
  bool contains(const K& key) const;

  V& operator[](const K& key);
  const std::optional<V> at(const K& key) const noexcept;

 public:
  void insert(const K& key, V value);
  void remove(const K& key);
  void erase(ConstIterator position);
  void erase(ConstIterator first, ConstIterator last);
  void clear();

 public:
  Iterator begin();
  ConstIterator begin() const;
  ConstIterator cbegin() const;

  Iterator end();
  ConstIterator end() const;
  ConstIterator cend() const;

  Iterator find(const K& key);
  ConstIterator find(const K& key) const;

 private:
  struct PairOrderingKey {
    const K& operator()(const Pair<const K, V>& data) const {
      return data.first;
    }
  };

  using MapTree = RBTree<Pair<const K, V>, less<const K>, PairOrderingKey>;

  using MapNode = MapTree::Node;

 private:
  MapTree tree_;

 public:
  class Iterator {
   public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = Pair<const K, V>;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type*;
    using reference = value_type&;

   public:
    Iterator() = default;
    ~Iterator() = default;
    Iterator(const Iterator&) = default;
    Iterator(Iterator&&) noexcept = default;
    Iterator& operator=(const Iterator&) = default;
    Iterator& operator=(Iterator&&) noexcept = default;

   public:
    Iterator(MapNode* node, MapTree* tree);

   public:
    Iterator& operator++();
    Iterator operator++(int);

    Iterator& operator--();
    Iterator operator--(int);

    reference operator*() const;
    pointer operator->() const;

    bool operator==(const Iterator& other) const;
    bool operator!=(const Iterator& other) const;

    bool operator==(const ConstIterator& other) const;
    bool operator!=(const ConstIterator& other) const;

   private:
    MapNode* curr_ = nullptr;
    MapTree* tree_ = nullptr;

   private:
    friend class Map<K, V>;
    friend class ConstIterator;
  };

  class ConstIterator {
   public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = Pair<const K, V>;
    using difference_type = std::ptrdiff_t;
    using pointer = const value_type*;
    using reference = const value_type&;

   public:
    ConstIterator() = default;
    ~ConstIterator() = default;
    ConstIterator(const ConstIterator&) = default;
    ConstIterator(ConstIterator&&) noexcept = default;
    ConstIterator& operator=(const ConstIterator&) = default;
    ConstIterator& operator=(ConstIterator&&) noexcept = default;

   public:
    ConstIterator(const Iterator& it);
    ConstIterator(const MapNode* node, const MapTree* tree);

   public:
    ConstIterator& operator++();
    ConstIterator operator++(int);

    ConstIterator& operator--();
    ConstIterator operator--(int);

    // TODO(fix this. Should not be const)
    reference operator*() const;
    pointer operator->() const;

    bool operator==(const ConstIterator& other) const;
    bool operator!=(const ConstIterator& other) const;

    bool operator==(const Iterator& other) const;
    bool operator!=(const Iterator& other) const;

   private:
    const MapNode* curr_ = nullptr;
    const MapTree* tree_ = nullptr;

   private:
    friend class Map<K, V>;
    friend class Iterator;
  };
};

}  // namespace utils

}  // namespace json

#include "map.inl"
