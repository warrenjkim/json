#pragma once

#include <cstddef>
#include <string>
#include <type_traits>

#include "nodes/node.h"
#include "utils/map.h"

namespace json {

namespace visitors {

class IteratorVisitor;

}  // namespace visitors

class Value;

}  // namespace json

template <typename T>
concept ReasonableInteger =
    std::is_integral_v<T> && !std::is_same_v<T, bool> &&
    !std::is_same_v<T, char> && !std::is_same_v<T, wchar_t> &&
    !std::is_same_v<T, char8_t> && !std::is_same_v<T, char16_t> &&
    !std::is_same_v<T, char32_t> && !std::is_same_v<T, json::Value>;

template <typename T>
concept ReasonableNumber =
    std::is_arithmetic_v<T> && !std::is_same_v<T, bool> &&
    !std::is_same_v<T, json::Value>;

template <typename T>
concept ReasonableString =
    (std::is_same_v<T, std::string> || std::is_same_v<T, std::string_view> ||
     std::is_same_v<T, const char*>) &&
    !std::is_arithmetic_v<T> && !std::is_same_v<T, json::Value>;

namespace json {

class Value {
 public:
  class Iterator;
  class ConstIterator;

 public:
  Value();
  ~Value();
  Value(Value&& other);
  Value(const Value& other);
  Value& operator=(Value&& other);
  Value& operator=(const Value& other);

 public:
  Value(Node* node);
  Value(const nullptr_t);
  Value(const bool value);
  Value(const char* value);

 public:
  void push_back(const nullptr_t);
  void push_back(const bool value);
  void push_back(const char* value);
  void push_back(const Value& value);

 public:
  void insert(const std::string&, const nullptr_t);
  void insert(const std::string&, const bool value);
  void insert(const std::string&, const char* value);
  void insert(const std::string&, const Value& value);

 public:
  Iterator begin();
  Iterator end();

 public:
  operator bool() const;
  operator nullptr_t() const;
  operator const char*() const;

  Value& operator[](const char* key);

 public:
  Value& operator=(const bool value);
  Value& operator=(const char* value);
  Value& operator=(const nullptr_t value);

 public:
  const std::string to_string() const;

 public:
  friend bool operator==(const Value& lhs, const bool rhs);
  friend bool operator==(const Value& lhs, const char* rhs);
  friend bool operator==(const Value& lhs, const nullptr_t);
  friend bool operator==(const Value& lhs, const Value& rhs);
  friend bool operator==(const Value& lhs, const Array& rhs);
  friend bool operator==(const Value& lhs, const Object& rhs);

 public:
  template <ReasonableNumber T>
  Value(const T value);

  template <ReasonableString T>
  Value(const T& value);

 public:
  template <ReasonableNumber T>
  void push_back(const T value);

  template <ReasonableString T>
  void push_back(const T& value);

 public:
  template <ReasonableNumber T>
  void insert(const std::string& key, const T value);

  template <ReasonableString T>
  void insert(const std::string& key, const T& value);

  // TODO(add erase() once iterators are implemented)
 public:
  template <ReasonableInteger T>
  void remove(const T index);

  template <ReasonableString T>
  void remove(const T key);

 public:
  template <ReasonableNumber T>
  operator T() const;

  template <ReasonableString T>
  operator T() const;

 public:
  template <ReasonableInteger T>
  Value& operator[](const T index);

  template <ReasonableString T>
  Value& operator[](const T& key);

 public:
  template <ReasonableNumber T>
  Value& operator=(const T value);

  template <ReasonableString T>
  Value& operator=(const T& value);

 public:
  template <ReasonableNumber T>
  friend bool operator==(const Value& lhs, const T rhs);

  template <ReasonableString T>
  friend bool operator==(const Value& lhs, const T& rhs);

 private:
  Node* node_;
  Value* parent_;
  std::optional<std::string> key_;
  utils::Map<std::string, Value> cache_;

 public:
  class Iterator {
   private:
    union ContainerIterator;

   public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = Value;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type*;
    using reference = value_type&;

   public:
    Iterator();
    ~Iterator();

   public:
    Iterator(const Iterator&) = default;
    Iterator(Iterator&&) noexcept = default;
    Iterator& operator=(const Iterator&) = default;
    Iterator& operator=(Iterator&&) noexcept = default;

   public:
    Iterator(Node* node, Value* value, ContainerIterator& it);

   public:
    Iterator& operator++();
    Iterator operator++(int);

    Iterator& operator--();
    Iterator operator--(int);

    reference operator*() const;
    pointer operator->() const;

    bool operator==(const Iterator& other) const;
    bool operator!=(const Iterator& other) const;

   private:
    Value* curr_ = nullptr;
    Value* value_ = nullptr;
    union ContainerIterator {
      std::vector<Node*>::iterator* array_it;
      utils::Map<std::string, Node*>::Iterator* map_it;
    } it_;

   private:
    Iterator(Value* value);

   private:
    friend class Value;
    friend class ConstIterator;
    friend class visitors::IteratorVisitor;
  };

 private:
  friend class visitors::IteratorVisitor;
};

}  // namespace json

#include "json/value.inl"
