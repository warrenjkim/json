#include "warren/internal/dsa/numeric.h"

#include "gtest/gtest.h"

TEST(NumericTest, ToInt64OverflowThrows) {
  EXPECT_THROW(json::dsa::to_integral("9223372036854775808"),
               std::out_of_range);
}

TEST(NumericTest, ToInt64UnderflowThrows) {
  EXPECT_THROW(json::dsa::to_integral("-9223372036854775809"),
               std::out_of_range);
}

TEST(NumericTest, ToInt8Positive) {
  json::dsa::Integral i8 = json::dsa::to_integral("42");
  EXPECT_EQ(i8.type, json::dsa::Integral::INT8);
  EXPECT_EQ(i8.i8, 42);
}

TEST(NumericTest, ToInt8Negative) {
  json::dsa::Integral i8 = json::dsa::to_integral("-42");
  EXPECT_EQ(i8.type, json::dsa::Integral::INT8);
  EXPECT_EQ(i8.i8, -42);
}

TEST(NumericTest, ToInt16Promotion) {  // > INT8_MAX
  json::dsa::Integral i16 = json::dsa::to_integral("130");
  EXPECT_EQ(i16.type, json::dsa::Integral::INT16);
  EXPECT_EQ(i16.i16, 130);
}

TEST(NumericTest, ToInt16NegativePromotion) {  // < INT8_MIN
  json::dsa::Integral i16 = json::dsa::to_integral("-129");
  EXPECT_EQ(i16.type, json::dsa::Integral::INT16);
  EXPECT_EQ(i16.i16, -129);
}

TEST(NumericTest, ToInt32Promotion) {  // > INT16_MAX
  json::dsa::Integral i32 = json::dsa::to_integral("32768");
  EXPECT_EQ(i32.type, json::dsa::Integral::INT32);
  EXPECT_EQ(i32.i32, 32768);
}

TEST(NumericTest, ToInt32NegativePromotion) {  // < INT16_MIN
  json::dsa::Integral i32 = json::dsa::to_integral("-32769");
  EXPECT_EQ(i32.type, json::dsa::Integral::INT32);
  EXPECT_EQ(i32.i32, -32769);
}

TEST(NumericTest, ToInt64Promotion) {  // > INT32_MAX
  json::dsa::Integral i64 = json::dsa::to_integral("2147483648");
  EXPECT_EQ(i64.type, json::dsa::Integral::INT64);
  EXPECT_EQ(i64.i64, 2147483648LL);
}

TEST(NumericTest, ToInt64NegativePromotion) {  // < INT32_MIN
  json::dsa::Integral i64 = json::dsa::to_integral("-2147483649");
  EXPECT_EQ(i64.type, json::dsa::Integral::INT64);
  EXPECT_EQ(i64.i64, -2147483649LL);
}
