#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string_view>

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

  Integral operator-(const size_t rhs) const {
    int64_t lhs = 0;
    switch (type) {
      case Integral::INT8:
        lhs = i8;
        break;
      case Integral::INT16:
        lhs = i16;
        break;
      case Integral::INT32:
        lhs = i32;
        break;
      case Integral::INT64:
        lhs = i64;
        break;
    }

    if (rhs > size_t(INT64_MAX)) {
      throw std::out_of_range("rhs is too large for int64_t subtraction");
    }

    if (lhs < INT64_MIN + int64_t(rhs)) {
      throw std::out_of_range("Integral subtraction would underflow");
    }

    lhs -= int64_t(rhs);
    Integral res;
    if (lhs >= INT8_MIN && lhs <= INT8_MAX) {
      res.i8 = int8_t(lhs);
      res.type = Integral::INT8;
    } else if (lhs >= INT16_MIN && lhs <= INT16_MAX) {
      res.i16 = int16_t(lhs);
      res.type = Integral::INT16;
    } else if (lhs >= INT32_MIN && lhs <= INT32_MAX) {
      res.i32 = int32_t(lhs);
      res.type = Integral::INT32;
    } else {
      res.type = Integral::INT64;
      res.i64 = lhs;
    }

    return res;
  }
};

// TODO(i want to ultimately implement promotion logic from float to double, but
// i haven't implemented bigint yet, so i can't implement any clamping logic)
struct Numeric {
  enum { INTEGRAL, /* FLOAT, */ DOUBLE } type;

  union {
    Integral intgr;
    // float flt;
    double dbl;
  };

  Numeric(Integral&& intgr) : intgr(std::move(intgr)), type(INTEGRAL) {}
  // Numeric(float flt) : flt(flt), type(FLOAT) {}
  Numeric(double dbl) : dbl(dbl), type(DOUBLE) {}
};

Integral to_integral(std::string_view intgr);

Numeric to_numeric(std::string_view intgr, std::string_view frac,
                   std::string_view exp);
}  // namespace dsa

}  // namespace json
