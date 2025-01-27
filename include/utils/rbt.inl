#pragma once

#include "pair.h"
#include "rbt.h"

namespace json {

namespace utils {

template <typename K, typename V, class Comparator>
RBTree<K, V, Comparator>::~RBTree() noexcept {
  clear();
}

template <typename K, typename V, class Comparator>
RBTree<K, V, Comparator>::RBTree(const RBTree& other)
    : root_(other.root_ ? new Node(*other.root_) : nullptr),
      size_(other.size_) {}

template <typename K, typename V, class Comparator>
RBTree<K, V, Comparator>::RBTree(RBTree&& other) noexcept
    : root_(other.root_), size_(other.size_) {
  other.root_ = nullptr;
  other.size_ = 0;
}

template <typename K, typename V, class Comparator>
RBTree<K, V, Comparator>& RBTree<K, V, Comparator>::operator=(
    const RBTree& other) {
  if (this != &other) {
    clear();
    size_ = other.size_;
    if (other.root_) {
      root_ = new Node(*other.root_);
    }
  }

  return *this;
}

template <typename K, typename V, class Comparator>
RBTree<K, V, Comparator>& RBTree<K, V, Comparator>::operator=(
    RBTree&& other) noexcept {
  if (this != &other) {
    clear();
    root_ = other.root_;
    size_ = other.size_;
    other.root_ = nullptr;
    other.size_ = 0;
  }
  return *this;
}

template <typename K, typename V, class Comparator>
constexpr RBTree<K, V, Comparator>::Node*
RBTree<K, V, Comparator>::root() noexcept {
  return root_;
}

template <typename K, typename V, class Comparator>
constexpr const RBTree<K, V, Comparator>::Node* RBTree<K, V, Comparator>::root()
    const noexcept {
  return root_;
}

template <typename K, typename V, class Comparator>
RBTree<K, V, Comparator>::Node* RBTree<K, V, Comparator>::min() noexcept {
  Node* curr = root_;
  while (curr && curr->left) {
    curr = curr->left;
  }

  return curr;
}

template <typename K, typename V, class Comparator>
const RBTree<K, V, Comparator>::Node* RBTree<K, V, Comparator>::min()
    const noexcept {
  return const_cast<Node*>(const_cast<RBTree*>(this)->min());
}

template <typename K, typename V, class Comparator>
RBTree<K, V, Comparator>::Node* RBTree<K, V, Comparator>::max() noexcept {
  Node* curr = root_;
  while (curr && curr->right) {
    curr = curr->right;
  }

  return curr;
}

template <typename K, typename V, class Comparator>
const RBTree<K, V, Comparator>::Node* RBTree<K, V, Comparator>::max()
    const noexcept {
  return const_cast<Node*>(const_cast<RBTree*>(this)->max());
}

template <typename K, typename V, class Comparator>
RBTree<K, V, Comparator>::Node* RBTree<K, V, Comparator>::successor(
    Node* node) noexcept {
  if (!node) {
    return nullptr;
  }

  Node* curr = node;
  if (curr->right) {
    curr = curr->right;
    while (curr && curr->left) {
      curr = curr->left;
    }

    return curr;
  } else {
    while (curr->parent && curr == curr->parent->right) {
      curr = curr->parent;
    }

    return curr->parent;
  }
}

template <typename K, typename V, class Comparator>
const RBTree<K, V, Comparator>::Node* RBTree<K, V, Comparator>::successor(
    const Node* node) const noexcept {
  return const_cast<const Node*>(
      const_cast<RBTree*>(this)->successor(const_cast<Node*>(node)));
}

template <typename K, typename V, class Comparator>
RBTree<K, V, Comparator>::Node* RBTree<K, V, Comparator>::predecessor(
    Node* node) noexcept {
  if (!node) {
    return nullptr;
  }

  Node* curr = node;
  if (curr->left) {
    curr = curr->left;
    while (curr && curr->right) {
      curr = curr->right;
    }

    return curr;
  } else {
    while (curr->parent && curr == curr->parent->left) {
      curr = curr->parent;
    }

    return curr->parent;
  }
}

template <typename K, typename V, class Comparator>
const RBTree<K, V, Comparator>::Node* RBTree<K, V, Comparator>::predecessor(
    const Node* node) const noexcept {
  return const_cast<const Node*>(
      const_cast<RBTree*>(this)->predecessor(const_cast<Node*>(node)));
}

template <typename K, typename V, class Comparator>
constexpr bool RBTree<K, V, Comparator>::empty() const noexcept {
  return size_ == 0;
}

template <typename K, typename V, class Comparator>
constexpr size_t RBTree<K, V, Comparator>::size() const noexcept {
  return size_;
}

template <typename K, typename V, class Comparator>
void RBTree<K, V, Comparator>::insert(const K& key, const V& value) {
  Node* z = nullptr;
  const size_t size = size_;
  root_ = recursive_insert(root_, key, value, z);
  if (size_ == size) {
    return;
  }

  root_ = double_red_fixup(z);
}

template <typename K, typename V, class Comparator>
void RBTree<K, V, Comparator>::erase(const K& key) {
  Node* del = recursive_find(root_, key);
  if (!del) {
    return;
  }

  root_ = recursive_remove(del);
  size_--;
}

template <typename K, typename V, class Comparator>
void RBTree<K, V, Comparator>::clear() {
  recursive_clear(root_);
  root_ = nullptr;
  size_ = 0;
}

template <typename K, typename V, class Comparator>
RBTree<K, V, Comparator>::Node* RBTree<K, V, Comparator>::find(
    const K& key) noexcept {
  return recursive_find(root_, key);
}

template <typename K, typename V, class Comparator>
const RBTree<K, V, Comparator>::Node* RBTree<K, V, Comparator>::find(
    const K& key) const noexcept {
  return const_cast<const Node*>(recursive_find(const_cast<Node*>(root_), key));
}

template <typename K, typename V, class Comparator>
bool RBTree<K, V, Comparator>::operator==(
    const RBTree<K, V, Comparator>& other) const noexcept {
  return (size_ == other.size_) && (root_ && other.root_) &&
         (*root_ == *other.root_);
}

template <typename K, typename V, class Comparator>
bool RBTree<K, V, Comparator>::operator!=(
    const RBTree<K, V, Comparator>& other) const noexcept {
  return !(*this == other);
}

template <typename K, typename V, class Comparator>
RBTree<K, V, Comparator>::Node* RBTree<K, V, Comparator>::recursive_insert(
    Node* root, const K& key, V value, Node*& z) {
  if (!root) {
    size_++;
    z = new Node(key, value);
    return z;
  }

  if (key < root->data.first) {
    root->left = recursive_insert(root->left, key, value, z);
    if (root->left == z) {
      z->parent = root;
    }
  } else if (key > root->data.first) {
    root->right = recursive_insert(root->right, key, value, z);
    if (root->right == z) {
      z->parent = root;
    }
  } else {
    root->data.second = value;
  }

  return root;
}

template <typename K, typename V, class Comparator>
RBTree<K, V, Comparator>::Node* RBTree<K, V, Comparator>::recursive_remove(
    Node* target) {
  if (target->left && target->right) {
    Node* replacement = successor(target);
    target->data.~Pair();
    new (&target->data) Pair<K, V>(std::move(replacement->data));

    return recursive_remove(replacement);
  }

  Node* replacement = target->left ? target->left : target->right;
  Node* parent = target->parent;
  rbt::Color target_color = target->color;

  // relink parent to point to replacement
  if (parent) {
    if (parent->left == target) {
      parent->left = replacement;
    } else {
      parent->right = replacement;
    }
  } else {
    root_ = replacement;
  }

  // update replacement to point to parent
  if (replacement) {
    replacement->parent = parent;
  }

  delete target;

  if (target_color == rbt::Color::BLACK) {
    if (replacement && replacement->color == rbt::Color::RED) {
      replacement->color = rbt::Color::BLACK;
    } else {
      Node* z = double_black_fixup(replacement, parent);
      while (z && z->parent) {
        z = z->parent;
      }

      return z;
    }
  }

  return root_;
}

template <typename K, typename V, class Comparator>
RBTree<K, V, Comparator>::Node* RBTree<K, V, Comparator>::recursive_find(
    Node* root, const K& key) noexcept {
  if (!root) {
    return nullptr;
  }

  if (key < root->data.first) {
    return recursive_find(root->left, key);
  } else if (key > root->data.first) {
    return recursive_find(root->right, key);
  }

  return root;
}

template <typename K, typename V, class Comparator>
void RBTree<K, V, Comparator>::recursive_clear(Node* node) {
  if (!node) {
    return;
  }

  recursive_clear(node->left);
  recursive_clear(node->right);

  delete node;
}

template <typename K, typename V, class Comparator>
RBTree<K, V, Comparator>::Node* RBTree<K, V, Comparator>::double_red_fixup(
    Node* z) noexcept {
  if (!z->parent) {
    z->color = rbt::Color::BLACK;
    return z;
  }

  if (is_black(z) || is_black(z->parent)) {
    return double_red_fixup(z->parent);
  }

  Node* u = uncle(z);
  if (is_black(u)) {
    z = restructure(z);
    z->color = rbt::Color::BLACK;
    z->left->color = rbt::Color::RED;
    z->right->color = rbt::Color::RED;
    return double_red_fixup(z);
  }

  return double_red_fixup(recolor(z));
}

template <typename K, typename V, class Comparator>
RBTree<K, V, Comparator>::Node* RBTree<K, V, Comparator>::double_black_fixup(
    Node* replacement, Node* parent) noexcept {
  if (!parent) {
    return replacement;
  }

  Node* sibling = (parent->left == replacement) ? parent->right : parent->left;

  if (is_red(sibling)) {
    Node* z = (parent->left == sibling) ? sibling->left : sibling->right;
    z = restructure(z);
    z->color = rbt::Color::BLACK;
    parent->color = rbt::Color::RED;
    return double_black_fixup(replacement, parent);
  }

  if (is_black(sibling->left) && is_black(sibling->right)) {
    sibling->color = rbt::Color::RED;
    if (is_red(parent)) {
      parent->color = rbt::Color::BLACK;

      return parent;
    } else {
      return double_black_fixup(parent, parent->parent);
    }
  }

  Node* z = nullptr;
  if (is_red(sibling->left) && is_red(sibling->right)) {
    z = (parent->left == sibling) ? sibling->left : sibling->right;
  } else {
    z = is_red(sibling->left) ? sibling->left : sibling->right;
  }

  z = restructure(z);
  z->color = parent->color;
  z->left->color = rbt::Color::BLACK;
  z->right->color = rbt::Color::BLACK;

  return z;
}

template <typename K, typename V, class Comparator>
RBTree<K, V, Comparator>::Node* RBTree<K, V, Comparator>::recolor(
    Node* z) noexcept {
  z->parent->color = rbt::Color::BLACK;
  uncle(z)->color = rbt::Color::BLACK;
  grandparent(z)->color = rbt::Color::RED;

  return grandparent(z);
}

template <typename K, typename V, class Comparator>
RBTree<K, V, Comparator>::Node* RBTree<K, V, Comparator>::restructure(
    Node* z) noexcept {
  rbt::Structure structure;
  if (grandparent(z)->left == z->parent) {
    structure = z->parent->left == z ? rbt::Structure::LEFT_LEFT
                                     : rbt::Structure::LEFT_RIGHT;
  } else {
    structure = z->parent->left == z ? rbt::Structure::RIGHT_LEFT
                                     : rbt::Structure::RIGHT_RIGHT;
  }

  switch (structure) {
    case rbt::Structure::LEFT_LEFT:
      z = right_rotate(grandparent(z));
      break;
    case rbt::Structure::LEFT_RIGHT:
      z = right_rotate(left_rotate(z->parent)->parent);
      break;
    case rbt::Structure::RIGHT_LEFT:
      z = left_rotate(right_rotate(z->parent)->parent);
      break;
    case rbt::Structure::RIGHT_RIGHT:
      z = left_rotate(grandparent(z));
      break;
  }

  return z;
}

template <typename K, typename V, class Comparator>
RBTree<K, V, Comparator>::Node* RBTree<K, V, Comparator>::left_rotate(
    Node* node) noexcept {
  Node* new_root = node->right;
  node->right = new_root->left;
  if (new_root->left) {
    new_root->left->parent = node;
  }

  new_root->parent = node->parent;
  if (node->parent) {
    if (node->parent->left == node) {
      node->parent->left = new_root;
    } else {
      node->parent->right = new_root;
    }
  }

  new_root->left = node;
  node->parent = new_root;

  return new_root;
}

template <typename K, typename V, class Comparator>
RBTree<K, V, Comparator>::Node* RBTree<K, V, Comparator>::right_rotate(
    Node* node) noexcept {
  Node* new_root = node->left;

  node->left = new_root->right;
  if (new_root->right) {
    new_root->right->parent = node;
  }

  new_root->parent = node->parent;
  if (node->parent) {
    if (node->parent->left == node) {
      node->parent->left = new_root;
    } else {
      node->parent->right = new_root;
    }
  }

  new_root->right = node;
  node->parent = new_root;

  return new_root;
}

template <typename K, typename V, class Comparator>
constexpr RBTree<K, V, Comparator>::Node* RBTree<K, V, Comparator>::grandparent(
    Node* node) noexcept {
  return node->parent->parent;
}

template <typename K, typename V, class Comparator>
constexpr RBTree<K, V, Comparator>::Node* RBTree<K, V, Comparator>::uncle(
    Node* node) noexcept {
  Node* gp = grandparent(node);
  return gp->left == node->parent ? gp->right : gp->left;
}

template <typename K, typename V, class Comparator>
constexpr bool RBTree<K, V, Comparator>::is_black(
    const Node* node) const noexcept {
  return !node || node->color == rbt::Color::BLACK;
}

template <typename K, typename V, class Comparator>
constexpr bool RBTree<K, V, Comparator>::is_red(
    const Node* node) const noexcept {
  return node && node->color == rbt::Color::RED;
}

}  // namespace utils

}  // namespace json
