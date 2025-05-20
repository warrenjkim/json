#pragma once

#include <algorithm>  // max
#include <cmath>      // abs
#include <cstdint>
#include <stdexcept>  // runtime_error
#include <string_view>

#include "warren/internal/utils/concepts.h"

namespace json {

namespace dsa {

struct Integral {
  enum {
    INT8,   // char
    INT16,  // short
    INT32,  // int
    INT64   // long long
  } type;

  union {
    int8_t i8;
    int16_t i16;
    int32_t i32;
    int64_t i64;

    // used only during parsing
    uint64_t accumulator;
  };

  Integral() : i8(0), type(INT8) {}

  explicit Integral(const int64_t value) {
    if (value >= INT8_MIN && value <= INT8_MAX) {
      i8 = static_cast<int8_t>(value);
      type = INT8;
    } else if (value >= INT16_MIN && value <= INT16_MAX) {
      i16 = static_cast<int16_t>(value);
      type = INT16;
    } else if (value >= INT32_MIN && value <= INT32_MAX) {
      i32 = static_cast<int32_t>(value);
      type = INT32;
    } else {
      i64 = value;
      type = INT64;
    }
  }

  [[noreturn]] void unreachable() const {
    throw std::runtime_error("Invalid Integral");
  }

  template <ReasonableNumber T>
  explicit operator T() const {
    switch (type) {
      case INT8:
        return static_cast<T>(i8);
      case INT16:
        return static_cast<T>(i16);
      case INT32:
        return static_cast<T>(i32);
      case INT64:
        return static_cast<T>(i64);
    }

    unreachable();
  }

  bool operator==(const Integral& rhs) const {
    if (type != rhs.type) {
      return false;
    }

    switch (type) {
      case Integral::INT8:
        return i8 == rhs.i8;
      case Integral::INT16:
        return i16 == rhs.i16;
      case Integral::INT32:
        return i32 == rhs.i32;
      case Integral::INT64:
        return i64 == rhs.i64;
    }

    unreachable();
  }

  bool operator!=(const Integral& rhs) const { return !operator==(rhs); }
};

// TODO(i want to ultimately implement promotion logic from float to double, but
// i haven't implemented bigint yet, so i can't implement any clamping logic)
struct Numeric {
  enum { INTEGRAL, /* FLOAT, */ DOUBLE } type;

  union {
    Integral intgr;
    double dbl;
  };

  Numeric() : intgr(), type(INTEGRAL) {}
  Numeric(Integral&& intgr) : intgr(std::move(intgr)), type(INTEGRAL) {}
  Numeric(double dbl) : dbl(dbl), type(DOUBLE) {}

  [[noreturn]] void unreachable() const {
    throw std::runtime_error("Invalid Numeric");
  }

  template <ReasonableNumber T>
  explicit operator T() const {
    switch (type) {
      case INTEGRAL:
        return static_cast<T>(intgr);
      case DOUBLE:
        return static_cast<T>(dbl);
    }

    unreachable();
  }

  bool operator==(const Numeric& rhs) const {
    if (type != rhs.type) {
      return false;
    }

    switch (type) {
      case Numeric::INTEGRAL:
        return intgr == rhs.intgr;
      case Numeric::DOUBLE:
        return std::abs(dbl - rhs.dbl) <
               (1e-10 * std::max(std::abs(dbl), std::abs(rhs.dbl)));
    }

    unreachable();
  }

  bool operator!=(const Numeric& rhs) const { return !operator==(rhs); }
};

Integral to_integral(std::string_view intgr);

Numeric to_numeric(std::string_view intgr, std::string_view frac,
                   std::string_view exp);
}  // namespace dsa

}  // namespace json
