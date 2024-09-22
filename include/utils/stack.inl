#pragma once

#include <utility>  // for std::move

#include "stack.h"

namespace json {

namespace utils {

template <typename T>
Stack<T>::Node::Node(const T& data) : data(data), next(nullptr) {}

template <typename T>
Stack<T>::Node::Node(T&& data) : data(std::move(data)), next(nullptr) {}

template <typename T>
inline Stack<T>::Stack() : head_(nullptr), size_(0) {}

template <typename T>
inline Stack<T>::~Stack() {
  clear();
}

template <typename T>
inline Stack<T>::Stack(const Stack& other) : head_(nullptr), size_(0) {
  deep_copy(other);
}

template <typename T>
inline Stack<T>::Stack(Stack&& other) noexcept
    : head_(other.head_), size_(other.size_) {
  other.head_ = nullptr;
  other.size_ = 0;
}

template <typename T>
Stack<T>& Stack<T>::operator=(const Stack& other) {
  if (this != &other) {
    clear();
    deep_copy(other);
  }
  return *this;
}

template <typename T>
Stack<T>& Stack<T>::operator=(Stack&& other) noexcept {
  if (this != &other) {
    clear();
    head_ = other.head_;
    size_ = other.size_;
    other.head_ = nullptr;
    other.size_ = 0;
  }
  return *this;
}

template <typename T>
inline std::optional<T> Stack<T>::top() {
  if (empty()) {
    return std::nullopt;
  }
  return head_->data;
}

template <typename T>
inline const std::optional<T> Stack<T>::top() const {
  if (empty()) {
    return std::nullopt;
  }
  return head_->data;
}

template <typename T>
inline bool Stack<T>::empty() const {
  return size_ == 0;
}

template <typename T>
inline size_t Stack<T>::size() const {
  return size_;
}

template <typename T>
inline void Stack<T>::clear() {
  while (!empty()) {
    pop();
  }
}

template <typename T>
inline void Stack<T>::push(const T& data) {
  Node* node = new Node(data);
  node->next = head_;
  head_ = node;
  size_++;
}

template <typename T>
inline void Stack<T>::push(T&& data) {
  Node* node = new Node(std::move(data));
  node->next = head_;
  head_ = node;
  size_++;
}

template <typename T>
inline void Stack<T>::pop() {
  if (empty()) {
    return;
  }
  Node* del = head_;
  head_ = head_->next;
  delete del;
  size_--;
}

template <typename T>
void Stack<T>::deep_copy(const Stack& other) {
  if (other.empty()) {
    return;
  }
  Node* curr = other.head_;
  Node* prev = new Node(curr->data);
  head_ = prev;
  while ((curr = curr->next)) {
    Node* node = new Node(curr->data);
    prev->next = node;
    prev = node;
  }
  size_ = other.size_;
}

}  // namespace utils

}  // namespace json
