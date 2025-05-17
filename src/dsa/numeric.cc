#include "warren/internal/dsa/numeric.h"

#include <string>
#include <string_view>

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

  int sign = (-1 * (-1 * negative));
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

}  // namespace dsa

}  // namespace json
