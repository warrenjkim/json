#pragma once

#include <cstddef>
#include <optional>

namespace json {

namespace utils {

template <typename T>
class Stack {
 public:
  struct Node {
    T data;
    Node* next;
    Node(const T& data);
    Node(T&& data);
  };

 public:
  Stack();
  ~Stack();
  Stack(const Stack& other);
  Stack(Stack&& other) noexcept;
  Stack& operator=(const Stack& other);
  Stack& operator=(Stack&& other) noexcept;

 public:
  std::optional<T> top();
  const std::optional<T> top() const;
  bool empty() const;
  size_t size() const;

 public:
  void clear();
  void push(const T& data);
  void push(T&& data);
  void pop();

 private:
  Node* head_;
  size_t size_;

 private:
  void deep_copy(const Stack& other);
};

}  // namespace utils

}  // namespace json

#include "stack.inl"
