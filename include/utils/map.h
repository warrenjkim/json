#pragma once

#include <cstddef>
#include <optional>
#include <utility>  // move

#include "pair.h"

namespace json {

namespace utils {

namespace map {

enum class Color { RED, BLACK };

enum class Structure { LEFT_LEFT, RIGHT_RIGHT, LEFT_RIGHT, RIGHT_LEFT };

}  // namespace map

template <typename K, typename V>
class Map {
 public:
  struct Node {
    Pair<K, V> data;
    Node* left;
    Node* right;

    map::Color color;
    Node* parent;

    Node(K key, V value)
        : data({std::move(key), value}),
          color(map::Color::RED),
          left(nullptr),
          right(nullptr),
          parent(nullptr) {}

    Node(const Pair<K, V>& data)
        : data(data),
          color(map::Color::RED),
          left(nullptr),
          right(nullptr),
          parent(nullptr) {}

    ~Node() = default;
  };

 public:
  class Iterator;
  class ConstIterator;

 public:
  Map();
  ~Map();
  Map(const Map& other);
  Map(Map&& other) noexcept;

  Map& operator=(const Map& other);
  Map& operator=(Map&& other) noexcept;

  void swap(Map& other) noexcept;

 public:
  bool operator==(const Map& other) const;
  bool operator!=(const Map& other) const;

 public:
  Node* root();
  const Node* root() const;

  size_t size() const;
  bool empty() const;
  bool contains(const K& key) const;

  std::optional<V> get(const K& key);
  const std::optional<V> get(const K& key) const;

  V& operator[](const K& key);
  const V& operator[](const K& key) const;

 public:
  void insert(const K& key, V value);
  void remove(const K& key);
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
  Node* root_;
  size_t size_;

 private:
  Node* recursive_insert(Node* root, const K& key, V value, Node*& z);
  Node* recursive_remove(Node* key);
  Node* recursive_get(Node* root, const K& key);
  const Node* recursive_get(const Node* root, const K& key) const;
  void clear(Node* node);

 private:
  Node* double_red_fixup(Node* z);
  Node* double_black_fixup(Node* replacement, Node* parent);

 private:
  Node* recolor(Node* z);
  Node* restructure(Node* z);
  Node* left_rotate(Node* node);
  Node* right_rotate(Node* node);

 private:
  Node* tree_min(Node* node);
  const Node* tree_min(const Node* node) const;

  Node* tree_max(Node* node);
  const Node* tree_max(const Node* node) const;

  Node* grandparent(Node* node);
  Node* uncle(Node* node);

  Node* successor(Node* node);
  const Node* successor(const Node* node) const;

  Node* predecessor(Node* node);
  const Node* predecessor(const Node* node) const;

  bool is_black(const Node* node) const;
  bool is_red(const Node* node) const;

  void deep_copy(const Map& other);

 public:
  class Iterator {
   public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = Pair<const K, V>;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type*;
    using reference = value_type&;

   public:
    Iterator();
    ~Iterator() = default;
    Iterator(Node* node, const Map<K, V>* tree);

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
    Node* curr_;
    const Map<K, V>* tree_;
    friend class ConstIterator;
  };

  class ConstIterator {
   public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = Pair<const K, const V>;
    using difference_type = std::ptrdiff_t;
    using pointer = const value_type*;
    using reference = const value_type&;

   public:
    ConstIterator();
    ~ConstIterator() = default;
    ConstIterator(const Node* node, const Map<K, V>* tree);
    ConstIterator(const Iterator& it);

   public:
    ConstIterator& operator++();
    ConstIterator operator++(int);

    ConstIterator& operator--();
    ConstIterator operator--(int);

    reference operator*() const;
    pointer operator->() const;

    bool operator==(const ConstIterator& other) const;
    bool operator!=(const ConstIterator& other) const;

    bool operator==(const Iterator& other) const;
    bool operator!=(const Iterator& other) const;

   private:
    const Node* curr_;
    const Map<K, V>* tree_;
    friend class Iterator;
  };
};

}  // namespace utils

}  // namespace json

#include "map.inl"
