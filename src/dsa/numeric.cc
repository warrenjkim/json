#include "warren/internal/dsa/numeric.h"

#include <cstddef>  // size_t
#include <cstdint>
#include <cstring>  // memcpy
#include <string>
#include <string_view>
#include <utility>  // move

namespace {

#define GRS_BITS 3

#define FLOAT_EMIN 1
#define FLOAT_EMAX 254
#define FLOAT_BIAS 127
#define FLOAT_SIGNIFICAND_BITS 23

#define DOUBLE_EMIN 1
#define DOUBLE_EMAX 2046
#define DOUBLE_BIAS 1023
#define DOUBLE_SIGNIFICAND_BITS 52

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

Integral to_integral(std::string_view intgr) {
  Integral res;
  bool negative = false;
  for (const char c : intgr) {
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
          throw std::out_of_range("Number " + std::string(intgr) +
                                  " is out of range of int64_t limits");
        }

        res.accumulator *= 10;
        res.accumulator += digit;

        break;
    }
  }

  int8_t sign = negative ? -1 : 1;
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
  int64_t lhs = static_cast<int64_t>(to_integral(exp));
  size_t rhs = frac.length();

  if (rhs > size_t(INT64_MAX)) {
    throw std::out_of_range("rhs is too large for int64_t subtraction");
  }

  if (lhs < INT64_MIN + int64_t(rhs)) {
    throw std::out_of_range("Integral subtraction would underflow");
  }

  Integral exponent = Integral(lhs - int64_t(rhs));
  size_t i = 0;
  if (!intgr.empty() && intgr.front() == '-') {
    i++;
  }

  std::string digits = std::string(intgr.substr(i)) + std::string(frac);
  return FloatingPoint(i == 1, std::move(digits), exponent);
}

NormalizedFloatingPoint normalize(FloatingPoint&& fp) {
  uint64_t N = to_uint64(fp.digits);
  uint64_t D = 1;
  int64_t exp = 0;
  switch (fp.exponent.type) {
    case Integral::INT8:
      exp = fp.exponent.i8;
      break;
    case Integral::INT16:
      exp = fp.exponent.i16;
      break;
    case Integral::INT32:
      exp = fp.exponent.i32;
      break;
    case Integral::INT64:
      exp = fp.exponent.i64;
      break;
  }

  if (exp < 0) {
    D *= pow10(-exp);
  } else {
    N *= pow10(exp);
  }

  // normalize N and D s.t. N/D < 2
  int64_t exponent = 0;
  while (N >= (D << 1)) {
    D <<= 1;
    exponent++;
  }

  // normalize N and D s.t. N/D >= 1
  while (N < D) {
    N <<= 1;
    exponent--;
  }

  // stores exactly 64 bits of precision of the fractional part of the
  // binary expansion of N / D \in [1, 2).
  // the leading 1 is implicit in IEEE 754 ==> we don't need to store it.
  // for i \in [0, 64)
  //   b_i = \floor{(2 * r_i) / D}
  //   r_{i + 1} = (2 * r_i) - (b_i * D)
  uint64_t significand = 0;
  N -= D;
  for (size_t i = 0; i < 64; i++) {
    N <<= 1;
    significand <<= 1;

    if (N >= D) {
      significand |= 1;
      N -= D;
    }
  }

  return NormalizedFloatingPoint(fp.negative, significand, exponent);
}

// TODO(this is currently an internal function that nobody calls. since i
// haven't implemented bigint yet, i can't accurately determine if i should
// promote or not)
// std::optional<float> to_binary32(NormalizedFloatingPoint nfp) {
//   uint64_t mask = (1ULL << (64 - (FLOAT_SIGNIFICAND_BITS + GRS_BITS))) - 1;
//   uint64_t top = nfp.significand >> (64 - (FLOAT_SIGNIFICAND_BITS +
//   GRS_BITS));
//
//   uint32_t mantissa = top >> 3;
//   bool guard = (top >> 2) & 1;
//   bool round = (top >> 1) & 1;
//   bool sticky = ((top & 1) | (nfp.significand & mask));
//
//   // round up to the nearest even number
//   bool round_up = (guard && (round || sticky || (mantissa & 1)));
//   if (round_up) {
//     mantissa++;
//
//     if (mantissa == (1U << FLOAT_SIGNIFICAND_BITS)) {
//       // rounding overflowed into the implicit bit
//       mantissa = 0;
//       nfp.exponent++;
//     }
//   }
//
//   nfp.exponent += FLOAT_BIAS;
//   if (nfp.exponent > FLOAT_EMAX || nfp.exponent < FLOAT_EMIN) {
//     return std::nullopt;
//   }
//
//   uint32_t bits = (uint32_t(nfp.negative) << 31) |
//                   (uint32_t(nfp.exponent) << 23) | mantissa;
//
//   float res = 0;
//   memcpy(&res, &bits, sizeof(float));
//
//   return res;
// }

// TODO(i need to implement bigint before i can implement any clamping logic)
double to_binary64(NormalizedFloatingPoint nfp) {
  uint64_t mask = (1ULL << (64 - (DOUBLE_SIGNIFICAND_BITS + GRS_BITS))) - 1;
  uint64_t top = nfp.significand >> (64 - (DOUBLE_SIGNIFICAND_BITS + GRS_BITS));

  uint64_t mantissa = top >> 3;
  bool guard = (top >> 2) & 1;
  bool round = (top >> 1) & 1;
  bool sticky = ((top & 1) | (nfp.significand & mask));

  // round up to the nearest even number
  bool round_up = (guard && (round || sticky || (mantissa & 1)));
  if (round_up) {
    mantissa++;

    if (mantissa == (1ULL << DOUBLE_SIGNIFICAND_BITS)) {
      // rounding overflowed into the implicit bit
      mantissa = 0;
      nfp.exponent++;
    }
  }

  nfp.exponent += DOUBLE_BIAS;

  double res = 0;
  uint64_t bits = (uint64_t(nfp.negative) << 63) |
                  (uint64_t(nfp.exponent) << 52) | mantissa;

  memcpy(&res, &bits, sizeof(double));

  return res;
}

// TODO(find a way to parse into a float as well)
Numeric to_numeric(std::string_view intgr, std::string_view frac,
                   std::string_view exp) {
  if (frac.empty() && exp.empty()) {
    return Numeric(std::move(to_integral(intgr)));
  }

  return Numeric(to_binary64(
      std::move(normalize(std::move(to_floating_point(intgr, frac, exp))))));
}

}  // namespace dsa

}  // namespace json
