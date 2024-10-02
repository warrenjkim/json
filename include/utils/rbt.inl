#pragma once

#include <optional>
#include <stdexcept>

#include "pair.h"
#include "rbt.h"
#include "stack.h"

namespace json {

namespace utils {

template <typename K, typename V>
RBTree<K, V>::RBTree() : root_(nullptr), size_(0) {}

template <typename K, typename V>
RBTree<K, V>::~RBTree() {
  clear();
}

template <typename K, typename V>
RBTree<K, V>::RBTree(const RBTree& other) : root_(nullptr), size_(0) {
  deep_copy(other);
}

template <typename K, typename V>
RBTree<K, V>::RBTree(RBTree&& other) noexcept
    : root_(other.root_), size_(other.size_) {
  other.root_ = nullptr;
  other.size_ = 0;
}

template <typename K, typename V>
RBTree<K, V>& RBTree<K, V>::operator=(const RBTree& other) {
  if (this != &other) {
    clear();
    deep_copy(other);
  }
  return *this;
}

template <typename K, typename V>
RBTree<K, V>& RBTree<K, V>::operator=(RBTree&& other) noexcept {
  if (this != &other) {
    clear();
    root_ = other.root_;
    size_ = other.size_;
    other.root_ = nullptr;
    other.size_ = 0;
  }
  return *this;
}

template <typename K, typename V>
void RBTree<K, V>::swap(RBTree& other) noexcept {
  std::swap(root_, other.root_);
  std::swap(size_, other.size_);
}

template <typename K, typename V>
RBTree<K, V>::Node* RBTree<K, V>::root() {
  return const_cast<Node*>(static_cast<const RBTree*>(this)->root());
}

template <typename K, typename V>
const RBTree<K, V>::Node* RBTree<K, V>::root() const {
  return root_;
}

template <typename K, typename V>
size_t RBTree<K, V>::size() const {
  return size_;
}

template <typename K, typename V>
std::optional<V> RBTree<K, V>::get(const K& key) {
  Node* node = recursive_get(root_, key);
  if (!node) {
    return std::nullopt;
  }

  return node->data.second;
}

template <typename K, typename V>
const std::optional<V> RBTree<K, V>::get(const K& key) const {
  const Node* node = recursive_get(root_, key);
  if (!node) {
    return std::nullopt;
  }

  return node->data.second;
}

template <typename K, typename V>
V& RBTree<K, V>::operator[](const K& key) {
  Node* node = recursive_get(root_, key);
  if (node) {
    return node->data.second;
  }

  insert(key, V());

  return recursive_get(root_, key)->data.second;
}

template <typename K, typename V>
const V& RBTree<K, V>::operator[](const K& key) const {
  const Node* node = recursive_get(root_, key);
  if (node) {
    return node->data.second;
  }

  static const V default_value{};
  return default_value;
}

template <typename K, typename V>
void RBTree<K, V>::insert(const K& key, V value) {
  Node* z = nullptr;
  size_t size = size_;
  root_ = recursive_insert(root_, key, value, z);
  if (size_ == size) {
    return;
  }

  root_ = double_red_fixup(z);
}

template <typename K, typename V>
void RBTree<K, V>::remove(const K& key) {
  Node* del = recursive_get(root_, key);
  if (!del) {
    return;
  }

  root_ = recursive_remove(del);
  size_--;
}

template <typename K, typename V>
void RBTree<K, V>::clear() {
  clear(root_);
  root_ = nullptr;
  size_ = 0;
}

template <typename K, typename V>
typename RBTree<K, V>::Iterator RBTree<K, V>::begin() {
  return Iterator(tree_min(root_), this);
}

template <typename K, typename V>
typename RBTree<K, V>::ConstIterator RBTree<K, V>::begin() const {
  return ConstIterator(tree_min(root_), this);
}

template <typename K, typename V>
typename RBTree<K, V>::ConstIterator RBTree<K, V>::cbegin() const {
  return ConstIterator(tree_min(root_), this);
}

template <typename K, typename V>
typename RBTree<K, V>::Iterator RBTree<K, V>::end() {
  return Iterator(nullptr, this);
}

template <typename K, typename V>
typename RBTree<K, V>::ConstIterator RBTree<K, V>::end() const {
  return ConstIterator(nullptr, this);
}

template <typename K, typename V>
typename RBTree<K, V>::ConstIterator RBTree<K, V>::cend() const {
  return ConstIterator(nullptr, this);
}

template <typename K, typename V>
typename RBTree<K, V>::Iterator RBTree<K, V>::find(const K& key) {
  Node* node = recursive_get(root_, key);
  return node ? Iterator(node, this) : end();
}

template <typename K, typename V>
typename RBTree<K, V>::ConstIterator RBTree<K, V>::find(const K& key) const {
  const Node* node = recursive_get(root_, key);
  return node ? ConstIterator(node, this) : cend();
}

template <typename K, typename V>
RBTree<K, V>::Node* RBTree<K, V>::recursive_insert(Node* root, const K& key,
                                                   V value, Node*& z) {
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

template <typename K, typename V>
RBTree<K, V>::Node* RBTree<K, V>::recursive_remove(Node* target) {
  if (target->left && target->right) {
    Node* replacement = successor(target);
    std::swap(
        *reinterpret_cast<Pair<K, V>*>(const_cast<Pair<K, V>*>(&target->data)),
        *reinterpret_cast<Pair<K, V>*>(
            const_cast<Pair<K, V>*>(&replacement->data)));
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

template <typename K, typename V>
RBTree<K, V>::Node* RBTree<K, V>::recursive_get(Node* root, const K& key) {
  return const_cast<Node*>(
      static_cast<const RBTree*>(this)->recursive_get(root, key));
}

template <typename K, typename V>
const RBTree<K, V>::Node* RBTree<K, V>::recursive_get(const Node* root,
                                                      const K& key) const {
  if (!root) {
    return nullptr;
  }

  if (key < root->data.first) {
    return recursive_get(root->left, key);
  } else if (key > root->data.first) {
    return recursive_get(root->right, key);
  }

  return root;
}

template <typename K, typename V>
void RBTree<K, V>::clear(Node* node) {
  if (!node) {
    return;
  }

  clear(node->left);
  clear(node->right);

  delete node;
}

template <typename K, typename V>
RBTree<K, V>::Node* RBTree<K, V>::double_red_fixup(Node* z) {
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

template <typename K, typename V>
RBTree<K, V>::Node* RBTree<K, V>::double_black_fixup(Node* replacement,
                                                     Node* parent) {
  if (!parent) {
    return replacement;
  }

  Node* sibling = (parent->left == replacement) ? parent->right : parent->left;

  // case 1: red sibling
  if (is_red(sibling)) {
    Node* z = (parent->left == sibling) ? sibling->left : sibling->right;
    z = restructure(z);
    z->color = rbt::Color::BLACK;
    parent->color = rbt::Color::RED;
    return double_black_fixup(replacement, parent);
  }

  // case 2: black sibling with two black children
  if (is_black(sibling->left) && is_black(sibling->right)) {
    sibling->color = rbt::Color::RED;
    if (is_red(parent)) {
      parent->color = rbt::Color::BLACK;

      return parent;
    } else {
      return double_black_fixup(parent, parent->parent);
    }
  }

  // case 3: black sibling with at least one red child
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

template <typename K, typename V>
RBTree<K, V>::Node* RBTree<K, V>::recolor(Node* z) {
  z->parent->color = rbt::Color::BLACK;
  uncle(z)->color = rbt::Color::BLACK;
  grandparent(z)->color = rbt::Color::RED;

  return grandparent(z);
}

template <typename K, typename V>
RBTree<K, V>::Node* RBTree<K, V>::restructure(Node* z) {
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

template <typename K, typename V>
RBTree<K, V>::Node* RBTree<K, V>::left_rotate(Node* node) {
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

template <typename K, typename V>
RBTree<K, V>::Node* RBTree<K, V>::right_rotate(Node* node) {
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

template <typename K, typename V>
RBTree<K, V>::Node* RBTree<K, V>::tree_min(Node* node) {
  return const_cast<Node*>(static_cast<const RBTree*>(this)->tree_min(node));
}

template <typename K, typename V>
const RBTree<K, V>::Node* RBTree<K, V>::tree_min(const Node* node) const {
  while (node && node->left) {
    node = node->left;
  }

  return node;
}

template <typename K, typename V>
RBTree<K, V>::Node* RBTree<K, V>::tree_max(Node* node) {
  return const_cast<Node*>(static_cast<const RBTree*>(this)->tree_max(node));
}

template <typename K, typename V>
const RBTree<K, V>::Node* RBTree<K, V>::tree_max(const Node* node) const {
  while (node && node->right) {
    node = node->right;
  }

  return node;
}

template <typename K, typename V>
RBTree<K, V>::Node* RBTree<K, V>::grandparent(Node* node) {
  return node->parent->parent;
}

template <typename K, typename V>
RBTree<K, V>::Node* RBTree<K, V>::uncle(Node* node) {
  Node* gp = grandparent(node);
  return gp->left == node->parent ? gp->right : gp->left;
}

template <typename K, typename V>
RBTree<K, V>::Node* RBTree<K, V>::successor(Node* node) {
  return const_cast<Node*>(static_cast<const RBTree*>(this)->successor(node));
}

template <typename K, typename V>
const typename RBTree<K, V>::Node* RBTree<K, V>::successor(
    const Node* node) const {
  if (node->right) {
    const Node* successor = node->right;
    while (successor->left) {
      successor = successor->left;
    }

    return successor;
  }

  const Node* successor = node;
  const Node* parent = node->parent;
  while (parent && successor == parent->right) {
    successor = parent;
    parent = parent->parent;
  }

  return parent;
}

template <typename K, typename V>
RBTree<K, V>::Node* RBTree<K, V>::predecessor(Node* node) {
  return const_cast<Node*>(static_cast<const RBTree*>(this)->predecessor(node));
}

template <typename K, typename V>
const RBTree<K, V>::Node* RBTree<K, V>::predecessor(const Node* node) const {
  if (node->left) {
    return tree_max(node->left);
  }

  while (node->parent && node == node->parent->left) {
    node = node->parent;
  }

  return node->parent;
}

template <typename K, typename V>
bool RBTree<K, V>::is_black(const Node* node) const {
  return !node || node->color == rbt::Color::BLACK;
}

template <typename K, typename V>
bool RBTree<K, V>::is_red(const Node* node) const {
  return node && node->color == rbt::Color::RED;
}

template <typename K, typename V>
void RBTree<K, V>::deep_copy(const RBTree& other) {
  clear();
  if (!other.root_) {
    return;
  }

  struct CopyNode {
    const Node* src;
    Node* dest;
    CopyNode(const Node* s, Node* d) : src(s), dest(d) {}
  };

  Stack<CopyNode> stack;
  root_ = new Node(other.root_->data);
  root_->color = other.root_->color;
  stack.push(CopyNode(other.root_, root_));

  while (!stack.empty()) {
    CopyNode current = *stack.top();
    stack.pop();

    if (current.src->left) {
      current.dest->left = new Node(current.src->left->data);
      current.dest->left->color = current.src->left->color;
      current.dest->left->parent = current.dest;
      stack.push(CopyNode(current.src->left, current.dest->left));
    }

    if (current.src->right) {
      current.dest->right = new Node(current.src->right->data);
      current.dest->right->color = current.src->right->color;
      current.dest->right->parent = current.dest;
      stack.push(CopyNode(current.src->right, current.dest->right));
    }
  }

  size_ = other.size_;
}

}  // namespace utils

}  // namespace json

namespace json {

namespace utils {

template <typename K, typename V>
RBTree<K, V>::Iterator::Iterator() : curr_(nullptr), tree_(nullptr) {}

template <typename K, typename V>
RBTree<K, V>::Iterator::Iterator(Node* node, const RBTree<K, V>* tree)
    : curr_(node), tree_(tree) {}

template <typename K, typename V>
typename RBTree<K, V>::Iterator& RBTree<K, V>::Iterator::operator++() {
  if (!curr_) {
    throw std::out_of_range("Iterator cannot be dereferenced");
  }

  curr_ = const_cast<Node*>(tree_->successor(curr_));
  return *this;
}

template <typename K, typename V>
typename RBTree<K, V>::Iterator RBTree<K, V>::Iterator::operator++(int) {
  Iterator temp = *this;
  ++(*this);
  return temp;
}

template <typename K, typename V>
typename RBTree<K, V>::Iterator& RBTree<K, V>::Iterator::operator--() {
  if (!tree_) {
    throw std::out_of_range("Iterator cannot be dereferenced");
  }

  if (!curr_) {
    curr_ = const_cast<Node*>(tree_->tree_max(tree_->root()));
  } else if (curr_ == tree_->tree_min(tree_->root())) {
    curr_ = nullptr;
  } else {
    curr_ = const_cast<Node*>(tree_->predecessor(curr_));
  }

  return *this;
}

template <typename K, typename V>
typename RBTree<K, V>::Iterator RBTree<K, V>::Iterator::operator--(int) {
  Iterator temp = *this;
  --(*this);
  return temp;
}

template <typename K, typename V>
typename RBTree<K, V>::Iterator::reference RBTree<K, V>::Iterator::operator*()
    const {
  if (!curr_) {
    throw std::out_of_range("Iterator cannot be dereferenced");
  }

  return reinterpret_cast<reference>(curr_->data);
}

template <typename K, typename V>
typename RBTree<K, V>::Iterator::pointer RBTree<K, V>::Iterator::operator->()
    const {
  return &(this->operator*());
}

template <typename K, typename V>
bool RBTree<K, V>::Iterator::operator==(const Iterator& other) const {
  return curr_ == other.curr_;
}

template <typename K, typename V>
bool RBTree<K, V>::Iterator::operator!=(const Iterator& other) const {
  return !(*this == other);
}

template <typename K, typename V>
bool RBTree<K, V>::Iterator::operator==(const ConstIterator& other) const {
  return curr_ == other.curr_;
}

template <typename K, typename V>
bool RBTree<K, V>::Iterator::operator!=(const ConstIterator& other) const {
  return !(*this == other);
}

template <typename K, typename V>
RBTree<K, V>::ConstIterator::ConstIterator() : curr_(nullptr), tree_(nullptr) {}

template <typename K, typename V>
RBTree<K, V>::ConstIterator::ConstIterator(const Node* node,
                                           const RBTree<K, V>* tree)
    : curr_(node), tree_(tree) {}

template <typename K, typename V>
RBTree<K, V>::ConstIterator::ConstIterator(const Iterator& it)
    : curr_(it.curr_), tree_(it.tree_) {}

template <typename K, typename V>
typename RBTree<K, V>::ConstIterator&
RBTree<K, V>::ConstIterator::operator++() {
  if (!curr_) {
    throw std::out_of_range("ConstIterator cannot be dereferenced");
  }

  curr_ = tree_->successor(curr_);
  return *this;
}

template <typename K, typename V>
typename RBTree<K, V>::ConstIterator RBTree<K, V>::ConstIterator::operator++(
    int) {
  ConstIterator temp = *this;
  ++(*this);
  return temp;
}

template <typename K, typename V>
typename RBTree<K, V>::ConstIterator&
RBTree<K, V>::ConstIterator::operator--() {
  if (!tree_) {
    throw std::out_of_range("Iterator cannot be dereferenced");
  }

  if (!curr_) {
    curr_ = tree_->tree_max(tree_->root());
  } else if (curr_ == tree_->tree_min(tree_->root())) {
    curr_ = nullptr;
  } else {
    curr_ = tree_->predecessor(curr_);
  }

  return *this;
}

template <typename K, typename V>
typename RBTree<K, V>::ConstIterator RBTree<K, V>::ConstIterator::operator--(
    int) {
  ConstIterator temp = *this;
  --(*this);
  return temp;
}

template <typename K, typename V>
typename RBTree<K, V>::ConstIterator::reference
RBTree<K, V>::ConstIterator::operator*() const {
  if (!curr_) {
    throw std::out_of_range("ConstIterator cannot be dereferenced");
  }

  return reinterpret_cast<reference>(curr_->data);
}

template <typename K, typename V>
typename RBTree<K, V>::ConstIterator::pointer
RBTree<K, V>::ConstIterator::operator->() const {
  return &(this->operator*());
}

template <typename K, typename V>
bool RBTree<K, V>::ConstIterator::operator==(const ConstIterator& other) const {
  return curr_ == other.curr_;
}

template <typename K, typename V>
bool RBTree<K, V>::ConstIterator::operator!=(const ConstIterator& other) const {
  return !(*this == other);
}

template <typename K, typename V>
bool RBTree<K, V>::ConstIterator::operator==(const Iterator& other) const {
  return curr_ == other.curr_;
}

template <typename K, typename V>
bool RBTree<K, V>::ConstIterator::operator!=(const Iterator& other) const {
  return !(*this == other);
}

}  // namespace utils

}  // namespace json
