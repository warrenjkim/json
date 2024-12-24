#pragma once

#include <cstddef>  // size_t
#include <optional>
#include <string>
#include <type_traits>  // concept

#include "nodes/node.h"
#include "utils/rbt.h"

namespace json {

class Value;

namespace visitors {

class GetVisitor;

class SetVisitor;

}  // namespace visitors

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
  Value();
  Value(Node* node);
  ~Value();

 public:
  Value(const nullptr_t);
  Value(const bool value);
  Value(const char* value);

 public:
  Value(const Value& other);
  Value(Value&& other);

 public:
  void add(const nullptr_t);
  void add(const bool value);
  void add(const char* value);
  void add(const Value& value);

 public:
  void put(const std::string&, const nullptr_t);
  void put(const std::string&, const bool value);
  void put(const std::string&, const char* value);
  void put(const std::string&, const Value& value);

 public:
  operator bool() const;
  operator nullptr_t() const;
  operator const char*() const;

 public:
  Value& operator=(Value&& value);
  Value& operator=(const nullptr_t);
  Value& operator=(const bool value);
  Value& operator=(const char* value);
  Value& operator=(const Value& value);

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
  void add(const T value);

  template <ReasonableString T>
  void add(const T& value);

 public:
  template <ReasonableNumber T>
  void put(const std::string& key, const T value);

  template <ReasonableString T>
  void put(const std::string& key, const T& value);

 public:
  template <ReasonableNumber T>
  operator T() const;

  template <ReasonableString T>
  operator T() const;

 public:
  template <ReasonableInteger T>
  Value& operator[](const T index);

  template <ReasonableInteger T>
  Value& operator[](const T index) const;

  template <ReasonableString T>
  Value& operator[](const T key);

  template <ReasonableString T>
  Value& operator[](const T& key) const;

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
  std::optional<size_t> index_;
  utils::Map<std::string, Value*> cache_;

 private:
  Value(Node* node, Value* parent, std::optional<size_t> index = std::nullopt);
};

}  // namespace json

#include "json/value.inl"
