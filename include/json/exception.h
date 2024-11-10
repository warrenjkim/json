#pragma once

#include <exception>
#include <string>

namespace json {

class JsonException : public std::exception {
 public:
  explicit JsonException(const std::string& message) : message_(message) {}

  const char* what() const noexcept override { return message_.c_str(); }

 private:
  std::string message_;
};

class BadAccessException : public JsonException {
  using JsonException::JsonException;
};

class NotFoundException : public JsonException {
  using JsonException::JsonException;
};

class BadCastException : public JsonException {
  using JsonException::JsonException;
};

}  // namespace json
