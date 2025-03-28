#pragma once

#include "warren/internal/dsa/rbt.h"

namespace json {

namespace dsa {

template <typename T, class Comparator, class OrderingKey>
RBTree<T, Comparator, OrderingKey>::~RBTree() noexcept {
  clear();
}

template <typename T, class Comparator, class OrderingKey>
RBTree<T, Comparator, OrderingKey>::RBTree(const RBTree& other)
    : root_(other.root_ ? new Node(*other.root_) : nullptr),
      size_(other.size_) {}

template <typename T, class Comparator, class OrderingKey>
RBTree<T, Comparator, OrderingKey>::RBTree(RBTree&& other) noexcept
    : root_(other.root_), size_(other.size_) {
  other.root_ = nullptr;
  other.size_ = 0;
}

template <typename T, class Comparator, class OrderingKey>
RBTree<T, Comparator, OrderingKey>&
RBTree<T, Comparator, OrderingKey>::operator=(const RBTree& other) {
  if (this != &other) {
    clear();
    size_ = other.size_;
    if (other.root_) {
      root_ = new Node(*other.root_);
    }
  }

  return *this;
}

template <typename T, class Comparator, class OrderingKey>
RBTree<T, Comparator, OrderingKey>&
RBTree<T, Comparator, OrderingKey>::operator=(RBTree&& other) noexcept {
  if (this != &other) {
    clear();
    root_ = other.root_;
    size_ = other.size_;
    other.root_ = nullptr;
    other.size_ = 0;
  }
  return *this;
}

template <typename T, class Comparator, class OrderingKey>
constexpr RBTree<T, Comparator, OrderingKey>::Node*
RBTree<T, Comparator, OrderingKey>::root() noexcept {
  return root_;
}

template <typename T, class Comparator, class OrderingKey>
constexpr const RBTree<T, Comparator, OrderingKey>::Node*
RBTree<T, Comparator, OrderingKey>::root() const noexcept {
  return root_;
}

template <typename T, class Comparator, class OrderingKey>
RBTree<T, Comparator, OrderingKey>::Node*
RBTree<T, Comparator, OrderingKey>::min() noexcept {
  Node* curr = root_;
  while (curr && curr->left) {
    curr = curr->left;
  }

  return curr;
}

template <typename T, class Comparator, class OrderingKey>
const RBTree<T, Comparator, OrderingKey>::Node*
RBTree<T, Comparator, OrderingKey>::min() const noexcept {
  return const_cast<Node*>(const_cast<RBTree*>(this)->min());
}

template <typename T, class Comparator, class OrderingKey>
RBTree<T, Comparator, OrderingKey>::Node*
RBTree<T, Comparator, OrderingKey>::max() noexcept {
  Node* curr = root_;
  while (curr && curr->right) {
    curr = curr->right;
  }

  return curr;
}

template <typename T, class Comparator, class OrderingKey>
const RBTree<T, Comparator, OrderingKey>::Node*
RBTree<T, Comparator, OrderingKey>::max() const noexcept {
  return const_cast<Node*>(const_cast<RBTree*>(this)->max());
}

template <typename T, class Comparator, class OrderingKey>
RBTree<T, Comparator, OrderingKey>::Node*
RBTree<T, Comparator, OrderingKey>::successor(Node* node) noexcept {
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

template <typename T, class Comparator, class OrderingKey>
const RBTree<T, Comparator, OrderingKey>::Node*
RBTree<T, Comparator, OrderingKey>::successor(const Node* node) const noexcept {
  return const_cast<const Node*>(
      const_cast<RBTree*>(this)->successor(const_cast<Node*>(node)));
}

template <typename T, class Comparator, class OrderingKey>
RBTree<T, Comparator, OrderingKey>::Node*
RBTree<T, Comparator, OrderingKey>::predecessor(Node* node) noexcept {
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

template <typename T, class Comparator, class OrderingKey>
const RBTree<T, Comparator, OrderingKey>::Node*
RBTree<T, Comparator, OrderingKey>::predecessor(
    const Node* node) const noexcept {
  return const_cast<const Node*>(
      const_cast<RBTree*>(this)->predecessor(const_cast<Node*>(node)));
}

template <typename T, class Comparator, class OrderingKey>
constexpr bool RBTree<T, Comparator, OrderingKey>::empty() const noexcept {
  return size_ == 0;
}

template <typename T, class Comparator, class OrderingKey>
constexpr size_t RBTree<T, Comparator, OrderingKey>::size() const noexcept {
  return size_;
}

template <typename T, class Comparator, class OrderingKey>
void RBTree<T, Comparator, OrderingKey>::insert(const T& data) {
  Node* z = nullptr;
  const size_t size = size_;
  root_ = recursive_insert(root_, data, &z);
  if (size_ == size) {
    return;
  }

  root_ = double_red_fixup(z);
}

template <typename T, class Comparator, class OrderingKey>
void RBTree<T, Comparator, OrderingKey>::erase(const T& data) {
  Node* del = recursive_find(root_, data);
  if (!del) {
    return;
  }

  root_ = recursive_erase(del);
  size_--;
}

template <typename T, class Comparator, class OrderingKey>
void RBTree<T, Comparator, OrderingKey>::clear() {
  recursive_clear(root_);
  root_ = nullptr;
  size_ = 0;
}

template <typename T, class Comparator, class OrderingKey>
RBTree<T, Comparator, OrderingKey>::Node*
RBTree<T, Comparator, OrderingKey>::find(const T& data) noexcept {
  return recursive_find(root_, data);
}

template <typename T, class Comparator, class OrderingKey>
const RBTree<T, Comparator, OrderingKey>::Node*
RBTree<T, Comparator, OrderingKey>::find(const T& data) const noexcept {
  return const_cast<const Node*>(
      const_cast<RBTree*>(this)->recursive_find(root_, data));
}

template <typename T, class Comparator, class OrderingKey>
bool RBTree<T, Comparator, OrderingKey>::operator==(
    const RBTree<T, Comparator, OrderingKey>& other) const noexcept {
  return (size_ == other.size_) && (root_ && other.root_) &&
         (*root_ == *other.root_);
}

template <typename T, class Comparator, class OrderingKey>
bool RBTree<T, Comparator, OrderingKey>::operator!=(
    const RBTree<T, Comparator, OrderingKey>& other) const noexcept {
  return !(*this == other);
}

template <typename T, class Comparator, class OrderingKey>
RBTree<T, Comparator, OrderingKey>::Node*
RBTree<T, Comparator, OrderingKey>::recursive_insert(Node* root, const T& data,
                                                     Node** z) {
  if (!root) {
    size_++;
    *z = new Node(data);
    return *z;
  }

  Comparator less;
  OrderingKey ordering_key;
  if (less(ordering_key(data), ordering_key(root->data))) {
    root->left = recursive_insert(root->left, data, z);
    if (root->left == *z) {
      (*z)->parent = root;
    }
  } else if (less(ordering_key(root->data), ordering_key(data))) {
    root->right = recursive_insert(root->right, data, z);
    if (root->right == *z) {
      (*z)->parent = root;
    }
  } else {
    root->data.~T();
    new (&root->data) T(data);
  }

  return root;
}

template <typename T, class Comparator, class OrderingKey>
RBTree<T, Comparator, OrderingKey>::Node*
RBTree<T, Comparator, OrderingKey>::recursive_erase(Node* target) {
  if (target->left && target->right) {
    Node* replacement = successor(target);
    target->data.~T();
    new (&target->data) T(std::move(replacement->data));

    return recursive_erase(replacement);
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

template <typename T, class Comparator, class OrderingKey>
RBTree<T, Comparator, OrderingKey>::Node*
RBTree<T, Comparator, OrderingKey>::recursive_find(Node* root,
                                                   const T& data) noexcept {
  if (!root) {
    return nullptr;
  }

  Comparator less;
  OrderingKey ordering_key;
  if (less(ordering_key(data), ordering_key(root->data))) {
    return recursive_find(root->left, data);
  } else if (less(ordering_key(root->data), ordering_key(data))) {
    return recursive_find(root->right, data);
  }

  return root;
}

template <typename T, class Comparator, class OrderingKey>
void RBTree<T, Comparator, OrderingKey>::recursive_clear(Node* node) {
  if (!node) {
    return;
  }

  recursive_clear(node->left);
  recursive_clear(node->right);

  delete node;
}

template <typename T, class Comparator, class OrderingKey>
RBTree<T, Comparator, OrderingKey>::Node*
RBTree<T, Comparator, OrderingKey>::double_red_fixup(Node* z) noexcept {
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

template <typename T, class Comparator, class OrderingKey>
RBTree<T, Comparator, OrderingKey>::Node*
RBTree<T, Comparator, OrderingKey>::double_black_fixup(Node* replacement,
                                                       Node* parent) noexcept {
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

template <typename T, class Comparator, class OrderingKey>
RBTree<T, Comparator, OrderingKey>::Node*
RBTree<T, Comparator, OrderingKey>::recolor(Node* z) noexcept {
  z->parent->color = rbt::Color::BLACK;
  uncle(z)->color = rbt::Color::BLACK;
  grandparent(z)->color = rbt::Color::RED;

  return grandparent(z);
}

template <typename T, class Comparator, class OrderingKey>
RBTree<T, Comparator, OrderingKey>::Node*
RBTree<T, Comparator, OrderingKey>::restructure(Node* z) noexcept {
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

template <typename T, class Comparator, class OrderingKey>
RBTree<T, Comparator, OrderingKey>::Node*
RBTree<T, Comparator, OrderingKey>::left_rotate(Node* node) noexcept {
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

template <typename T, class Comparator, class OrderingKey>
RBTree<T, Comparator, OrderingKey>::Node*
RBTree<T, Comparator, OrderingKey>::right_rotate(Node* node) noexcept {
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

template <typename T, class Comparator, class OrderingKey>
constexpr RBTree<T, Comparator, OrderingKey>::Node*
RBTree<T, Comparator, OrderingKey>::grandparent(Node* node) noexcept {
  return node->parent->parent;
}

template <typename T, class Comparator, class OrderingKey>
constexpr RBTree<T, Comparator, OrderingKey>::Node*
RBTree<T, Comparator, OrderingKey>::uncle(Node* node) noexcept {
  Node* gp = grandparent(node);
  return gp->left == node->parent ? gp->right : gp->left;
}

template <typename T, class Comparator, class OrderingKey>
constexpr bool RBTree<T, Comparator, OrderingKey>::is_black(
    const Node* node) const noexcept {
  return !node || node->color == rbt::Color::BLACK;
}

template <typename T, class Comparator, class OrderingKey>
constexpr bool RBTree<T, Comparator, OrderingKey>::is_red(
    const Node* node) const noexcept {
  return node && node->color == rbt::Color::RED;
}

}  // namespace dsa

}  // namespace json
