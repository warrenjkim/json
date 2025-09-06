#pragma once

#include <cstddef>
#include <cstdint>
#include <map>
#include <new>
#include <string>
#include <vector>

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
          // Nothing to copy
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
          // Nothing to move
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

  Value(bool b) noexcept : b_(b), type_(Type::BOOLEAN) {}

  Value(double n) noexcept : n_(n), type_(Type::DOUBLE) {}

  Value(int64_t n) noexcept : n_((double)n), type_(Type::INTEGRAL) {}

  Value(nullptr_t) noexcept : type_(Type::JSON_NULL) {}

  Value(const char* s) {
    ::new ((void*)(&s_)) std::string(s);
    type_ = Type::STRING;
  }

  Value(std::string s) noexcept {
    ::new ((void*)(&s_)) std::string(std::move(s));
    type_ = Type::STRING;
  }

  Value& operator=(const char* s) { return *this = std::string(s); }

  Value& operator=(std::string s) noexcept {
    destroy();
    ::new ((void*)(&s_)) std::string(std::move(s));
    type_ = Type::STRING;

    return *this;
  }

  Value(array_t a) {
    ::new ((void*)(&a_)) array_t(std::move(a));
    type_ = Type::ARRAY;
  }

  Value& operator=(array_t a) {
    destroy();
    ::new ((void*)(&a_)) array_t(std::move(a));
    type_ = Type::ARRAY;

    return *this;
  }

  Value(object_t o) {
    ::new ((void*)(&o_)) object_t(std::move(o));
    type_ = Type::OBJECT;
  }

  Value& operator=(object_t o) {
    destroy();
    ::new ((void*)(&o_)) object_t(std::move(o));
    type_ = Type::OBJECT;

    return *this;
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
};

}  // namespace json
