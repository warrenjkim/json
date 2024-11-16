#pragma once

#include <cstddef>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

#include "nodes/node.h"
#include "nodes/null.h"
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

 public:
  operator bool() const;
  operator const char*() const;
  operator nullptr_t() const;

 public:
  friend bool operator==(const Value& lhs, const Value& rhs);

  friend bool operator==(const Value& lhs, bool rhs);
  friend bool operator==(bool lhs, const Value& rhs);

  friend bool operator==(const Value& lhs, const char* rhs);
  friend bool operator==(const char* lhs, const Value& rhs);

  friend bool operator==(const Value& lhs, nullptr_t rhs);
  friend bool operator==(nullptr_t lhs, const Value& rhs);

 public:
  template <ReasonableInteger T>
  Value operator[](const T index);

  template <ReasonableString T>
  Value operator[](const T key);

 public:
  template <ReasonableNumber T>
  operator T() const;

  template <ReasonableString T>
  operator T() const;

 public:
  template <ReasonableNumber T>
  friend bool operator==(const Value& lhs, const T& rhs);

  template <ReasonableNumber T>
  friend bool operator==(const T& lhs, const Value& rhs);

  template <ReasonableString T>
  friend bool operator==(const Value& lhs, const T& rhs);

  template <ReasonableString T>
  friend bool operator==(const T& lhs, const Value& rhs);

 private:
  enum class Type { ARRAY, BOOLEAN, NUMBER, OBJECT, JSON_NULL, STRING };
  union Data {
    std::vector<Node*>* array;
    bool* boolean;
    double* number;
    utils::Map<std::string, Node*>* object;
    Null* null;
    std::string* string;
    Data() : null(new Null()) {}
  };

  Data data_;
  Type type_;
  Node* node_;

 private:
  friend class visitors::GetVisitor;
  friend class visitors::SetVisitor;
};

}  // namespace json

#include "json/value.inl"
