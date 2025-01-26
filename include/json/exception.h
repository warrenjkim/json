#pragma once

#include <exception>
#include <string>

// TODO(probably best to explain what these are, even though these are pretty
// straightforward)
namespace json {

class JsonException : public std::exception {
 public:
  explicit JsonException(const std::string& message) : message_(message) {}

  const char* what() const noexcept override { return message_.c_str(); }

 private:
  std::string message_;
};

class ParseException : public JsonException {
  using JsonException::JsonException;
};

class BadCastException : public JsonException {
  using JsonException::JsonException;
};

class BadAccessException : public JsonException {
  using JsonException::JsonException;
};

class UnexpectedParentException : public JsonException {
  using JsonException::JsonException;
};

}  // namespace json
