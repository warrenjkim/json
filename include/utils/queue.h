#pragma once

#include <optional>

namespace json {

namespace utils {

template <typename T>
class Queue {
 public:
  struct Node {
    T data;
    Node* next;
    Node* prev;
    Node(const T& data);
    Node(T&& data);

    template <typename... Args>
    Node(Args&&... args);
  };

 public:
  Queue();
  ~Queue();
  Queue(const Queue& other);
  Queue(Queue&& other) noexcept;
  Queue& operator=(const Queue& other);
  Queue& operator=(Queue&& other) noexcept;

 public:
  std::optional<T> front();
  const std::optional<T> front() const;

 public:
  bool empty() const;
  size_t size() const;

 public:
  void clear();
  void enqueue(const T& data);
  void enqueue(T&& data);
  std::optional<T> dequeue();

 public:
  template <typename... Args>
  void emplace(Args&&... args);

 private:
  Node* head_;
  Node* tail_;
  size_t size_;

 private:
  void deep_copy(const Queue& other);
};

}  // namespace utils

}  // namespace json

#include "queue.inl"
