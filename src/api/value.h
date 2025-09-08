#pragma once

#include <cstddef>
#include <cstdint>
#include <map>
#include <new>
#include <string>
#include <vector>

#include "warren/json/utils/exception.h"

namespace {
#define PRIMITIVE_TYPE_CTOR(type, v_, expected) \
  Value(type v) noexcept : v_(v), type_(expected) {}

#define IMPLICIT_MUTABLE_CONVERSION(expected, type, member) \
  operator type() {                                         \
    assert_type(expected);                                  \
    return member;                                          \
  }

#define IMPLICIT_CONVERSION(expected, type, member) \
  operator type() const {                           \
    assert_type(expected);                          \
    return member;                                  \
  }

#define PRIMITIVE_EQUALITY(expected, type, v_) \
  bool operator==(type v) const { return type_ == expected && v == v_; }
}  // namespace

namespace json {

class Value {
 private:
  using array_t = std::vector<json::Value>;
  using object_t = std::map<std::string, json::Value>;

 public:
  Value() noexcept : type_(Type::JSON_NULL) {}

  ~Value() noexcept { destroy(); }

  Value(const Value& other) {
    switch (other.type_) {
      case Type::ARRAY:
        ::new ((void*)(&a_)) array_t(other.a_);
        break;
      case Type::BOOLEAN:
        b_ = other.b_;
        break;
      case Type::JSON_NULL:
        break;
      case Type::INTEGRAL:
      case Type::DOUBLE:
        n_ = other.n_;
        break;
      case Type::OBJECT:
        ::new ((void*)(&o_)) object_t(other.o_);
        break;
      case Type::STRING:
        ::new ((void*)(&s_)) std::string(other.s_);
        break;
    }

    type_ = other.type_;
  }

  Value(Value&& other) noexcept {
    switch (other.type_) {
      case Type::ARRAY:
        ::new ((void*)(&a_)) array_t(std::move(other.a_));
        break;
      case Type::BOOLEAN:
        b_ = other.b_;
        break;
      case Type::JSON_NULL:
        break;
      case Type::INTEGRAL:
      case Type::DOUBLE:
        n_ = other.n_;
        break;
      case Type::OBJECT:
        ::new ((void*)(&o_)) object_t(std::move(other.o_));
        break;
      case Type::STRING:
        ::new ((void*)(&s_)) std::string(std::move(other.s_));
        break;
    }

    type_ = other.type_;
    other.type_ = Type::JSON_NULL;
  }

  Value(nullptr_t) noexcept : type_(Type::JSON_NULL) {}

  PRIMITIVE_TYPE_CTOR(bool, b_, Type::BOOLEAN);

  PRIMITIVE_TYPE_CTOR(int32_t, n_, Type::INTEGRAL);

  PRIMITIVE_TYPE_CTOR(double, n_, Type::DOUBLE);

  Value(array_t a) {
    ::new ((void*)(&a_)) array_t(std::move(a));
    type_ = Type::ARRAY;
  }

  Value(object_t o) {
    ::new ((void*)(&o_)) object_t(std::move(o));
    type_ = Type::OBJECT;
  }

  Value(const char* s) {
    ::new ((void*)(&s_)) std::string(s);
    type_ = Type::STRING;
  }

  Value(std::string s) noexcept {
    ::new ((void*)(&s_)) std::string(std::move(s));
    type_ = Type::STRING;
  }

  Value& operator=(const Value& other) {
    if (this != &other) {
      destroy();
      switch (other.type_) {
        case Type::ARRAY:
          ::new ((void*)(&a_)) array_t(other.a_);
          break;
        case Type::BOOLEAN:
          b_ = other.b_;
          break;
        case Type::JSON_NULL:
          break;
        case Type::INTEGRAL:
        case Type::DOUBLE:
          n_ = other.n_;
          break;
        case Type::OBJECT:
          ::new ((void*)(&o_)) object_t(other.o_);
          break;
        case Type::STRING:
          ::new ((void*)(&s_)) std::string(other.s_);
          break;
      }

      type_ = other.type_;
    }

    return *this;
  }

  Value& operator=(Value&& other) noexcept {
    if (this != &other) {
      destroy();
      switch (other.type_) {
        case Type::ARRAY:
          ::new ((void*)(&a_)) array_t(std::move(other.a_));
          break;
        case Type::BOOLEAN:
          b_ = other.b_;
          break;
        case Type::JSON_NULL:
          break;
        case Type::INTEGRAL:
        case Type::DOUBLE:
          n_ = other.n_;
          break;
        case Type::OBJECT:
          ::new ((void*)(&o_)) object_t(std::move(other.o_));
          break;
        case Type::STRING:
          ::new ((void*)(&s_)) std::string(std::move(other.s_));
          break;
      }

      type_ = other.type_;
      other.type_ = Type::JSON_NULL;
    }

    return *this;
  }

  IMPLICIT_CONVERSION(Type::ARRAY, const array_t&, a_);
  IMPLICIT_MUTABLE_CONVERSION(Type::ARRAY, array_t&, a_);

  IMPLICIT_CONVERSION(Type::BOOLEAN, bool, b_);

  IMPLICIT_CONVERSION(Type::DOUBLE, double, n_);
  IMPLICIT_CONVERSION(Type::DOUBLE, float, float(n_));

  IMPLICIT_CONVERSION(Type::INTEGRAL, int32_t, int32_t(n_));

  IMPLICIT_CONVERSION(Type::OBJECT, const object_t&, o_);
  IMPLICIT_MUTABLE_CONVERSION(Type::OBJECT, object_t&, o_);

  IMPLICIT_CONVERSION(Type::STRING, std::string, s_);
  IMPLICIT_CONVERSION(Type::STRING, const std::string&, s_);
  IMPLICIT_CONVERSION(Type::STRING, const char*, s_.c_str());

  bool operator==(const Value& other) const {
    if (type_ != other.type_) {
      return false;
    }

    switch (type_) {
      case Type::ARRAY:
        return a_ == other.a_;
      case Type::BOOLEAN:
        return b_ == other.b_;
      case Type::JSON_NULL:
        return true;
      case Type::INTEGRAL:
      case Type::DOUBLE:
        return n_ == other.n_;
      case Type::OBJECT:
        return o_ == other.o_;
      case Type::STRING:
        return s_ == other.s_;
    }
  }

  bool operator==(nullptr_t) const { return type_ == Type::JSON_NULL; }

  PRIMITIVE_EQUALITY(Type::BOOLEAN, bool, b_);

  PRIMITIVE_EQUALITY(Type::DOUBLE, double, n_);

  PRIMITIVE_EQUALITY(Type::INTEGRAL, int32_t, int32_t(n_));

  PRIMITIVE_EQUALITY(Type::STRING, std::string, s_);

  PRIMITIVE_EQUALITY(Type::STRING, const char*, s_);

  // containers
  size_t size() const {
    switch (type_) {
      case Type::ARRAY:
        return a_.size();
      case Type::OBJECT:
        return o_.size();
      default:
        throw BadAccessException(
            "expected container type (array, object), got " + type(type_));
    }
  }

  bool empty() const {
    switch (type_) {
      case Type::ARRAY:
        return a_.empty();
      case Type::OBJECT:
        return o_.empty();
      default:
        throw BadAccessException(
            "expected container type (array, object), got " + type(type_));
    }
  }

  // array
  template <typename T>
  typename std::enable_if_t<std::is_integral_v<T>, Value&> operator[](T i) {
    assert_type(Type::ARRAY);
    return a_[array_t::size_type(i)];
  }

  template <typename T>
  typename std::enable_if_t<std::is_integral_v<T>, const Value&> operator[](
      T i) const {
    assert_type(Type::ARRAY);
    return a_[array_t::size_type(i)];
  }

  void push_back(const Value& value) {
    if (type_ == Type::JSON_NULL) {
      destroy();
      ::new ((void*)(&a_)) array_t();
      type_ = Type::ARRAY;
    }

    assert_type(Type::ARRAY);
    a_.push_back(value);
  }

  template <typename T>
  typename std::enable_if_t<std::is_integral_v<T>, void> erase(T i) {
    assert_type(Type::ARRAY);
    a_.erase(a_.begin() + array_t::difference_type(i));
  }

  // object
  template <typename T>
  typename std::enable_if_t<std::is_convertible_v<T, std::string>, Value&>
  operator[](const T& key) {
    if (type_ == Type::JSON_NULL) {
      destroy();
      ::new ((void*)(&o_)) object_t();
      type_ = Type::OBJECT;
    }

    assert_type(Type::OBJECT);
    return o_[key];
  }

  template <typename T>
  typename std::enable_if_t<std::is_convertible_v<T, std::string>, const Value&>
  at(const T& key) const {
    assert_type(Type::OBJECT);
    return o_.at(key);
  }

  void insert(const std::string& key, const Value& value) {
    if (type_ == Type::JSON_NULL) {
      destroy();
      ::new ((void*)(&o_)) object_t();
      type_ = Type::OBJECT;
    }

    assert_type(Type::OBJECT);
    o_.insert({key, value});
  }

  template <typename T>
  typename std::enable_if_t<std::is_convertible_v<T, std::string>, void> erase(
      const T& key) {
    assert_type(Type::OBJECT);
    o_.erase(key);
  }

  const Value& at(const char* key) const {
    assert_type(Type::OBJECT);
    return o_.at(key);
  }

  void insert(const char* key, const Value& value) {
    if (type_ == Type::JSON_NULL) {
      destroy();
      ::new ((void*)(&o_)) object_t();
      type_ = Type::OBJECT;
    }

    assert_type(Type::OBJECT);
    o_.insert({key, value});
  }

 private:
  void destroy() noexcept {
    switch (type_) {
      case Type::ARRAY:
        a_.~array_t();
        break;
      case Type::OBJECT:
        o_.~object_t();
        break;
      case Type::STRING:
        s_.std::string::~string();
        break;
      default:
        break;
    }

    type_ = Type::JSON_NULL;
  }

  union {
    array_t a_;
    bool b_;
    double n_;
    object_t o_;
    std::string s_;
  };

  enum Type {
    ARRAY,
    BOOLEAN,
    JSON_NULL,
    INTEGRAL,
    DOUBLE,
    OBJECT,
    STRING
  } type_;

  void assert_type(Type expected) const {
    if (type_ != expected) {
      throw BadAccessException("expected type " + type(expected) + ", got " +
                               type(type_));
    }
  }

  std::string type(Type type) const {
    switch (type) {
      case ARRAY:
        return "array";
      case BOOLEAN:
        return "boolean";
      case JSON_NULL:
        return "null";
      case INTEGRAL:
      case DOUBLE:
        return "number";
      case OBJECT:
        return "object";
      case STRING:
        return "string";
    }
  }
};

}  // namespace json
