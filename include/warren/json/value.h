#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "warren/internal/dsa/map.h"
#include "warren/internal/nodes/node.h"
#include "warren/internal/utils/concepts.h"

namespace json {

namespace visitors {

class ContainerTypeVisitor;

}  // namespace visitors

class Value;

}  // namespace json

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
  Value(nodes::Node* node);
  Value(const nullptr_t);
  Value(const bool value);
  Value(const char* value);

 public:
  bool empty() const noexcept;
  size_t size() const noexcept;

 public:
  void push_back(const nullptr_t);
  void push_back(const bool value);
  void push_back(const char* value);
  void push_back(const Value& value);
  void push_back(const nodes::Node* node);

 public:
  void insert(const std::string&, const nullptr_t);
  void insert(const std::string&, const bool value);
  void insert(const std::string&, const char* value);
  void insert(const std::string&, const Value& value);
  void insert(const std::string&, const nodes::Node* node);

 public:
  void erase(ConstIterator position);
  void erase(ConstIterator first, ConstIterator last);

 public:
  Iterator begin() noexcept;
  Iterator end() noexcept;

  ConstIterator cbegin() noexcept;
  ConstIterator cend() noexcept;

 public:
  operator bool() const;
  operator nullptr_t() const;
  operator const char*() const;

  Value& operator[](const char* key);

 public:
  Value& operator=(const bool value);
  Value& operator=(const char* value);
  Value& operator=(const nullptr_t value);
  Value& operator=(const nodes::Node* node);

 public:
  const std::string to_string() const;

 public:
  friend bool operator==(const Value& lhs, const bool rhs);
  friend bool operator==(const Value& lhs, const char* rhs);
  friend bool operator==(const Value& lhs, const nullptr_t);
  friend bool operator==(const Value& lhs, const Value& rhs);
  friend bool operator==(const Value& lhs, const nodes::Array& rhs);
  friend bool operator==(const Value& lhs, const nodes::Object& rhs);

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

 public:
  template <ReasonableInteger T>
  void erase(const T index);

  template <ReasonableString T>
  void erase(const T key);

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
  nodes::Node* node_;
  Value* parent_;
  std::optional<std::string> key_;
  dsa::Map<std::string, Value> cache_;

 private:
  enum ContainerType { ARRAY, OBJECT };

 public:
  class Iterator {
   public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = Value;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type*;
    using reference = value_type&;

   public:
    Iterator();
    ~Iterator();
    Iterator(const Iterator& other);
    Iterator(Iterator&& other) noexcept;
    Iterator& operator=(const Iterator& other);
    Iterator& operator=(Iterator&& other) noexcept;

   public:
    Iterator& operator++() noexcept;
    Iterator operator++(int) noexcept;

    Iterator& operator--() noexcept;
    Iterator operator--(int) noexcept;

    reference operator*() noexcept;
    pointer operator->() noexcept;

    bool operator==(const Iterator& other) const noexcept;
    bool operator!=(const Iterator& other) const noexcept;

   private:
    Value* curr_ = nullptr;
    Value* value_ = nullptr;
    ContainerType type_;
    union ContainerIterator {
      std::vector<nodes::Node*>::iterator array_it =
          std::vector<nodes::Node*>::iterator();
      dsa::Map<std::string, nodes::Node*>::Iterator map_it;

      ContainerIterator() {}
      ~ContainerIterator() {}
    } it_;
    enum StartPosition { BEGIN, END };

   private:
    Iterator(Value* value, const StartPosition pos);

   private:
    friend class Value;
    friend class ConstIterator;
    friend class visitors::ContainerTypeVisitor;
  };

 public:
  class ConstIterator {
   public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = Value;
    using difference_type = std::ptrdiff_t;
    using const_pointer = value_type*;
    using const_reference = value_type&;

   public:
    ConstIterator();
    ~ConstIterator();
    ConstIterator(const ConstIterator& other);
    ConstIterator(ConstIterator&& other) noexcept;
    ConstIterator& operator=(const ConstIterator& other);
    ConstIterator& operator=(ConstIterator&& other) noexcept;

   public:
    ConstIterator(const Iterator& it);

   public:
    ConstIterator& operator++() noexcept;
    ConstIterator operator++(int) noexcept;

    ConstIterator& operator--() noexcept;
    ConstIterator operator--(int) noexcept;

    const_reference operator*() noexcept;
    const_pointer operator->() noexcept;

    bool operator==(const ConstIterator& other) const noexcept;
    bool operator!=(const ConstIterator& other) const noexcept;

   private:
    Value* curr_ = nullptr;
    Value* value_ = nullptr;
    ContainerType type_;
    union ContainerConstIterator {
      std::vector<nodes::Node*>::const_iterator array_cit =
          std::vector<nodes::Node*>::const_iterator();
      dsa::Map<std::string, nodes::Node*>::ConstIterator map_cit;
      ContainerConstIterator() {}
      ~ContainerConstIterator() {}
    } cit_;
    enum StartPosition { CBEGIN, CEND };

   private:
    ConstIterator(Value* value, const StartPosition pos);

   private:
    friend class Value;
    friend class ConstConstIterator;
    friend class visitors::ContainerTypeVisitor;
  };

 private:
  friend class visitors::ContainerTypeVisitor;
};

}  // namespace json

#include "warren/internal/impl/value.inl"
