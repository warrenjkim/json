#pragma once

#include <optional>
#include <utility>  // move, forward

#include "warren/internal/dsa/queue.h"

namespace json {

namespace utils {

template <typename T>
Queue<T>::Node::Node(const T& data)
    : data(data), next(nullptr), prev(nullptr) {}

template <typename T>
Queue<T>::Node::Node(T&& data)
    : data(std::move(data)), next(nullptr), prev(nullptr) {}

template <typename T>
template <typename... Args>
Queue<T>::Node::Node(Args&&... args)
    : data(std::forward<Args&&>(args)...), next(nullptr), prev(nullptr) {}

template <typename T>
Queue<T>::Queue() : head_(nullptr), tail_(nullptr), size_(0) {}

template <typename T>
Queue<T>::~Queue() {
  clear();
}

template <typename T>
Queue<T>::Queue(const Queue& other) : head_(nullptr), size_(0) {
  deep_copy(other);
}

template <typename T>
Queue<T>::Queue(Queue&& other) noexcept
    : head_(other.head_), size_(other.size_) {
  other.head_ = nullptr;
  other.size_ = 0;
}

template <typename T>
Queue<T>& Queue<T>::operator=(const Queue& other) {
  if (this != &other) {
    clear();
    deep_copy(other);
  }
  return *this;
}

template <typename T>
Queue<T>& Queue<T>::operator=(Queue&& other) noexcept {
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
std::optional<T> Queue<T>::front() {
  if (empty()) {
    return std::nullopt;
  }

  return head_->data;
}

template <typename T>
const std::optional<T> Queue<T>::front() const {
  if (empty()) {
    return std::nullopt;
  }

  return head_->data;
}

template <typename T>
bool Queue<T>::empty() const {
  return size_ == 0;
}

template <typename T>
size_t Queue<T>::size() const {
  return size_;
}

template <typename T>
void Queue<T>::clear() {
  while (!empty()) {
    dequeue();
  }
}

template <typename T>
void Queue<T>::enqueue(const T& data) {
  if (empty()) {
    tail_ = new Node(data);
    head_ = tail_;
    size_++;
    return;
  }

  tail_->next = new Node(data);
  tail_->next->prev = tail_;
  tail_ = tail_->next;
  size_++;
}

template <typename T>
void Queue<T>::enqueue(T&& data) {
  if (empty()) {
    tail_ = new Node(std::move(data));
    head_ = tail_;
    size_++;
    return;
  }

  tail_->next = new Node(std::move(data));
  tail_->next->prev = tail_;
  tail_ = tail_->next;
  size_++;
}

template <typename T>
std::optional<T> Queue<T>::dequeue() {
  if (empty()) {
    return std::nullopt;
  }

  Node* del = head_;
  head_ = head_->next;
  T value = del->data;
  delete del;
  size_--;
  if (empty()) {
    head_ = nullptr;
    tail_ = nullptr;
  }

  return value;
}

template <typename T>
template <typename... Args>
void Queue<T>::emplace(Args&&... args) {
  if (empty()) {
    tail_ = new Node(std::forward<Args&&>(args)...);
    head_ = tail_;
    size_++;
    return;
  }

  tail_->next = new Node(std::forward<Args&&>(args)...);
  tail_->next->prev = tail_;
  tail_ = tail_->next;
  size_++;
}

template <typename T>
template <typename... Args>
Queue<T> Queue<T>::of(Args&&... args) {
  static_assert((std::is_convertible_v<Args, T> && ...),
                "All arguments must be convertible to queue type T");
  Queue<T> queue;
  (queue.emplace(std::forward<Args&&>(args)), ...);
  return queue;
}

template <typename T>
void Queue<T>::deep_copy(const Queue& other) {
  if (other.empty()) {
    head_ = nullptr;
    tail_ = nullptr;
    size_ = 0;
    return;
  }

  Node* curr = other.head_;
  Node* prev = new Node(curr->data);
  head_ = prev;

  while ((curr = curr->next)) {
    Node* node = new Node(curr->data);
    prev->next = node;
    node->prev = prev;
    prev = node;
  }

  tail_ = prev;
  size_ = other.size_;
}

}  // namespace utils

}  // namespace json
