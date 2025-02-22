#pragma once

#include <optional>
#include <stdexcept>
#include <vector>

#include "map.h"

namespace json {

namespace utils {

template <typename K, typename V>
bool Map<K, V>::operator==(const Map<K, V>& other) const {
  return tree_ == other.tree_;
}

template <typename K, typename V>
bool Map<K, V>::operator!=(const Map<K, V>& other) const {
  return !(*this == other);
}

template <typename K, typename V>
constexpr size_t Map<K, V>::size() const {
  return tree_.size();
}

template <typename K, typename V>
constexpr bool Map<K, V>::empty() const {
  return tree_.empty();
}

template <typename K, typename V>
bool Map<K, V>::contains(const K& key) const {
  return tree_.find({key, V()}) != nullptr;
}

template <typename K, typename V>
const std::optional<V> Map<K, V>::at(const K& key) const noexcept {
  const Map<K, V>::MapNode* node = tree_.find({key, V()});
  if (!node) {
    return std::nullopt;
  }

  return node->data.second;
}

template <typename K, typename V>
V& Map<K, V>::operator[](const K& key) {
  Map<K, V>::MapNode* node = tree_.find({key, V()});
  if (node) {
    return node->data.second;
  }

  insert(key, V());

  return tree_.find({key, V()})->data.second;
}

template <typename K, typename V>
void Map<K, V>::insert(const K& key, V value) {
  tree_.insert({key, value});
}

template <typename K, typename V>
void Map<K, V>::erase(const K& key) {
  tree_.erase({key, V()});
}

template <typename K, typename V>
void Map<K, V>::erase(ConstIterator position) {
  if (!position.curr_) {
    throw std::out_of_range("Cannot erase invalid iterator.");
  }

  tree_.erase({position->first, position->second});
}

template <typename K, typename V>
void Map<K, V>::erase(ConstIterator first, ConstIterator last) {
  if (first == last) {
    return;
  }

  if (!first.tree_ || !last.tree_ || first.tree_ != last.tree_) {
    throw std::invalid_argument("Iterators must be from same map.");
  }

  std::vector<K> keys;
  ConstIterator it = first;
  while (it != last) {
    if (it == end()) {
      throw std::invalid_argument("Invalid iterator range.");
    }

    keys.push_back(it->first);
    ++it;
  }

  for (const auto& key : keys) {
    erase(key);
  }
}

template <typename K, typename V>
void Map<K, V>::clear() {
  tree_.clear();
}

template <typename K, typename V>
Map<K, V>::Iterator Map<K, V>::begin() {
  return Iterator(tree_.min(), &tree_);
}

template <typename K, typename V>
Map<K, V>::ConstIterator Map<K, V>::begin() const {
  return ConstIterator(tree_.min(), &tree_);
}

template <typename K, typename V>
Map<K, V>::ConstIterator Map<K, V>::cbegin() const {
  return ConstIterator(tree_.min(), &tree_);
}

template <typename K, typename V>
Map<K, V>::Iterator Map<K, V>::end() {
  return Iterator(nullptr, &tree_);
}

template <typename K, typename V>
Map<K, V>::ConstIterator Map<K, V>::end() const {
  return ConstIterator(nullptr, &tree_);
}

template <typename K, typename V>
Map<K, V>::ConstIterator Map<K, V>::cend() const {
  return ConstIterator(nullptr, &tree_);
}

template <typename K, typename V>
Map<K, V>::Iterator Map<K, V>::find(const K& key) {
  Map<K, V>::MapNode* node = tree_.find({key, V()});
  return node ? Iterator(node, &tree_) : end();
}

template <typename K, typename V>
Map<K, V>::ConstIterator Map<K, V>::find(const K& key) const {
  const Map<K, V>::MapNode* node = tree_.find({key, V()});
  return node ? ConstIterator(node, &tree_) : cend();
}

}  // namespace utils

}  // namespace json

namespace json {

namespace utils {

template <typename K, typename V>
Map<K, V>::Iterator::Iterator(Map<K, V>::MapNode* node,
                              Map<K, V>::MapTree* tree)
    : curr_(node), tree_(tree) {}

template <typename K, typename V>
Map<K, V>::Iterator& Map<K, V>::Iterator::operator++() {
  if (!curr_) {
    throw std::out_of_range("Iterator cannot be dereferenced");
  }

  curr_ = tree_->successor(curr_);

  return *this;
}

template <typename K, typename V>
Map<K, V>::Iterator Map<K, V>::Iterator::operator++(int) {
  Iterator temp = *this;
  ++(*this);

  return temp;
}

template <typename K, typename V>
Map<K, V>::Iterator& Map<K, V>::Iterator::operator--() {
  if (!tree_) {
    throw std::out_of_range("Iterator cannot be dereferenced");
  }

  if (!curr_) {
    curr_ = tree_->max();
  } else if (curr_ == tree_->min()) {
    curr_ = nullptr;
  } else {
    curr_ = tree_->predecessor(curr_);
  }

  return *this;
}

template <typename K, typename V>
Map<K, V>::Iterator Map<K, V>::Iterator::operator--(int) {
  Iterator temp = *this;
  --(*this);

  return temp;
}

template <typename K, typename V>
Map<K, V>::Iterator::reference Map<K, V>::Iterator::operator*() const {
  if (!curr_) {
    throw std::out_of_range("Iterator cannot be dereferenced");
  }

  return curr_->data;
}

template <typename K, typename V>
Map<K, V>::Iterator::pointer Map<K, V>::Iterator::operator->() const {
  return &(operator*());
}

template <typename K, typename V>
bool Map<K, V>::Iterator::operator==(const Iterator& other) const {
  return curr_ == other.curr_;
}

template <typename K, typename V>
bool Map<K, V>::Iterator::operator!=(const Iterator& other) const {
  return !(*this == other);
}

template <typename K, typename V>
bool Map<K, V>::Iterator::operator==(const ConstIterator& other) const {
  return curr_ == other.curr_;
}

template <typename K, typename V>
bool Map<K, V>::Iterator::operator!=(const ConstIterator& other) const {
  return !(*this == other);
}

template <typename K, typename V>
Map<K, V>::ConstIterator::ConstIterator(const Map<K, V>::MapNode* node,
                                        const Map<K, V>::MapTree* tree)
    : curr_(node), tree_(tree) {}

template <typename K, typename V>
Map<K, V>::ConstIterator::ConstIterator(const Iterator& it)
    : curr_(it.curr_), tree_(it.tree_) {}

template <typename K, typename V>
Map<K, V>::ConstIterator& Map<K, V>::ConstIterator::operator++() {
  if (!curr_) {
    throw std::out_of_range("ConstIterator cannot be dereferenced");
  }

  curr_ = tree_->successor(curr_);

  return *this;
}

template <typename K, typename V>
Map<K, V>::ConstIterator Map<K, V>::ConstIterator::operator++(int) {
  ConstIterator temp = *this;
  ++(*this);

  return temp;
}

template <typename K, typename V>
Map<K, V>::ConstIterator& Map<K, V>::ConstIterator::operator--() {
  if (!tree_) {
    throw std::out_of_range("Iterator cannot be dereferenced");
  }

  if (!curr_) {
    curr_ = tree_->max();
  } else if (curr_ == tree_->min()) {
    curr_ = nullptr;
  } else {
    curr_ = tree_->predecessor(curr_);
  }

  return *this;
}

template <typename K, typename V>
Map<K, V>::ConstIterator Map<K, V>::ConstIterator::operator--(int) {
  ConstIterator temp = *this;
  --(*this);

  return temp;
}

template <typename K, typename V>
Map<K, V>::ConstIterator::reference Map<K, V>::ConstIterator::operator*()
    const {
  if (!curr_) {
    throw std::out_of_range("ConstIterator cannot be dereferenced");
  }

  return curr_->data;
}

template <typename K, typename V>
Map<K, V>::ConstIterator::pointer Map<K, V>::ConstIterator::operator->() const {
  return &(operator*());
}

template <typename K, typename V>
bool Map<K, V>::ConstIterator::operator==(const ConstIterator& other) const {
  return curr_ == other.curr_;
}

template <typename K, typename V>
bool Map<K, V>::ConstIterator::operator!=(const ConstIterator& other) const {
  return !(*this == other);
}

template <typename K, typename V>
bool Map<K, V>::ConstIterator::operator==(const Iterator& other) const {
  return curr_ == other.curr_;
}

template <typename K, typename V>
bool Map<K, V>::ConstIterator::operator!=(const Iterator& other) const {
  return !(*this == other);
}

}  // namespace utils

}  // namespace json
