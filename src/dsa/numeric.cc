#include "warren/internal/dsa/numeric.h"

#include <cstdint>
#include <string>
#include <string_view>

namespace {

// semantic representation of the floating point (denormalized)
struct FloatingPoint {
  bool negative;
  std::string digits;            // intgr + frac (base 10)
  json::dsa::Integral exponent;  // adjusted exponent (base 10)

  FloatingPoint(const bool negative, std::string digits,
                json::dsa::Integral exponent)
      : negative(negative), digits(std::move(digits)), exponent(exponent) {}
};

// normalized representation of the floating point s.t.
// value = (-1)^{negative} * (significand / 2^{64}) * 2^{exponent}
struct NormalizedFloatingPoint {
  bool negative;
  uint64_t significand;  // fully binary expanded significand (64-bit precision)
  int64_t exponent;      // unbiased exponent (base 2)

  NormalizedFloatingPoint(const bool negative, const uint64_t significand,
                          const int64_t exponent)
      : negative(negative), significand(significand), exponent(exponent) {}
};

}  // namespace

namespace {

uint64_t pow10(int64_t n) {
  if (n >= 20) {
    throw std::out_of_range("Target number is out of range of uint64_t limits");
  }

  uint64_t res = 1;
  uint64_t base = 10;
  while (n > 0) {
    if (n & 1) {
      res *= base;
    }

    base *= base;
    n /= 2;
  }

  return res;
}

uint64_t to_uint64(std::string_view digits) {
  uint64_t res = 0;
  for (const char c : digits) {
    if (res > UINT64_MAX / 10) {
      throw std::out_of_range("Number " + std::string(digits) +
                              " is out of range of uint64_t limits");
    }

    res *= 10;
    if (res > UINT64_MAX - (c - '0')) {
      throw std::out_of_range("Number " + std::string(digits) +
                              " is out of range of uint64_t limits");
    }

    res += (c - '0');
  }

  return res;
}

}  // namespace

namespace json {

namespace dsa {

Integral to_integral(std::string_view sv) {
  Integral res;
  bool negative = false;
  for (const char c : sv) {
    if (c == '-') {
      negative = true;
      continue;
    } else if (c == '+') {
      continue;
    }

    int8_t digit = c - '0';
    switch (res.type) {
      case Integral::INT8:
        res.accumulator *= 10;
        res.accumulator += digit;
        if ((!negative && res.accumulator > INT8_MAX) ||
            (negative && res.accumulator > -INT8_MIN)) {
          res.type = Integral::INT16;
        }

        break;
      case Integral::INT16:
        res.accumulator *= 10;
        res.accumulator += digit;
        if ((!negative && res.accumulator > INT16_MAX) ||
            (negative && res.accumulator > -INT16_MIN)) {
          res.type = Integral::INT32;
        }

        break;
      case Integral::INT32:
        res.accumulator *= 10;
        res.accumulator += digit;
        if ((!negative && res.accumulator > INT32_MAX) ||
            (negative && res.accumulator > uint32_t(INT32_MAX) + 1)) {
          res.type = Integral::INT64;
        }

        break;
      case Integral::INT64:
        if ((!negative && res.accumulator > (INT64_MAX - digit) / 10) ||
            (negative &&
             res.accumulator > (uint64_t(INT64_MAX) + 1 - digit) / 10)) {
          throw std::out_of_range("Number " + std::string(sv) +
                                  " is out of range of int64_t limits");
        }

        res.accumulator *= 10;
        res.accumulator += digit;

        break;
    }
  }

  int sign = (-1 * (-1 + negative));
  switch (res.type) {
    case Integral::INT8:
      res.i8 = sign * int8_t(res.accumulator);
      break;
    case Integral::INT16:
      res.i16 = sign * int16_t(res.accumulator);
      break;
    case Integral::INT32:
      res.i32 = sign * int32_t(res.accumulator);
      break;
    case Integral::INT64:
      res.i64 = sign * int64_t(res.accumulator);
      break;
  }

  return res;
}

FloatingPoint to_floating_point(std::string_view intgr, std::string_view frac,
                                std::string_view exp) {
  Integral exponent = to_integral(exp) - frac.length();
  size_t i = 0;
  if (!intgr.empty() && intgr.front() == '-') {
    i++;
  }

  std::string digits = std::string(intgr.substr(i)) + std::string(frac);
  return FloatingPoint(i == 1, std::move(digits), exponent);
}

}  // namespace dsa

}  // namespace json
