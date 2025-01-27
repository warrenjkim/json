#pragma once

#include <cstddef>  // size_t
#include <utility>  // move

#include "pair.h"

namespace json {

namespace utils {

template <typename T>
struct less;

namespace rbt {

enum class Color { RED, BLACK };

enum class Structure { LEFT_LEFT, RIGHT_RIGHT, LEFT_RIGHT, RIGHT_LEFT };

}  // namespace rbt

template <typename K, typename V, class Comparator = less<Pair<K, V>>>
class RBTree {
 public:
  struct Node {
    Pair<K, V> data;
    Node* left;
    Node* right;

    rbt::Color color;
    Node* parent;

    Node(const Pair<K, V>& data)
        : data(data),
          color(rbt::Color::RED),
          left(nullptr),
          right(nullptr),
          parent(nullptr) {}

    Node(const Node& other)
        : data(other.data),
          color(other.color),
          left(other.left ? new Node(*other.left) : nullptr),
          right(other.right ? new Node(*other.right) : nullptr),
          parent(nullptr) {
      if (left) {
        left->parent = this;
      }
      if (right) {
        right->parent = this;
      }
    }

    Node(K key, V value) : Node(Pair<K, V>(std::move(key), value)) {}

    ~Node() = default;

    Node(Node&& other) noexcept = delete;
    Node& operator=(const Node& other) = delete;
    Node& operator=(Node&& other) noexcept = delete;
  };

 public:
  RBTree() noexcept = default;
  ~RBTree() noexcept;

  RBTree(const RBTree& other);
  RBTree(RBTree&& other) noexcept;

  RBTree& operator=(const RBTree& other);
  RBTree& operator=(RBTree&& other) noexcept;

 public:
  constexpr Node* root() noexcept;
  constexpr const Node* root() const noexcept;

  Node* min() noexcept;
  const Node* min() const noexcept;

  Node* max() noexcept;
  const Node* max() const noexcept;

  Node* successor(Node* node) noexcept;
  const Node* successor(const Node* node) const noexcept;

  Node* predecessor(Node* node) noexcept;
  const Node* predecessor(const Node* node) const noexcept;

 public:
  constexpr bool empty() const noexcept;
  constexpr size_t size() const noexcept;

 public:
  void insert(const K& key, const V& value);
  void erase(const K& key);
  void clear();
  Node* find(const K& key) noexcept;
  const Node* find(const K& key) const noexcept;

 public:
  bool operator==(const RBTree& other) const noexcept;
  bool operator!=(const RBTree& other) const noexcept;

 private:
  Node* root_ = nullptr;
  size_t size_ = 0;

 private:
  Node* recursive_insert(Node* root, const K& key, V value, Node*& z);

  Node* recursive_remove(Node* key);

  Node* recursive_find(Node* root, const K& key) noexcept;

  void recursive_clear(Node* node);

 private:
  Node* double_red_fixup(Node* z) noexcept;
  Node* double_black_fixup(Node* replacement, Node* parent) noexcept;

 private:
  Node* recolor(Node* z) noexcept;
  Node* restructure(Node* z) noexcept;

  Node* left_rotate(Node* node) noexcept;
  Node* right_rotate(Node* node) noexcept;

 private:
  void deep_copy(const RBTree& other);

  constexpr Node* uncle(Node* node) noexcept;
  constexpr Node* grandparent(Node* node) noexcept;

  constexpr bool is_black(const Node* node) const noexcept;
  constexpr bool is_red(const Node* node) const noexcept;
};

}  // namespace utils

}  // namespace json
