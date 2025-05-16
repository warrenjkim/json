#include <cstdint>

namespace json {

namespace dsa {

struct Integral {
  enum {
    UNSET,
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

  Integral() : i8(0), type(UNSET) {}
};

struct Numeric {
  enum { UNSET, INTEGRAL, FLOAT, DOUBLE } type;

  union {
    Integral integer;
    float flt;
    double dbl;
  };

  Numeric() : integer(), type(UNSET) {}
};

}  // namespace dsa

}  // namespace json
