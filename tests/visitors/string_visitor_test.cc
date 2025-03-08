#include "visitors/string_visitor.h"

#include <gtest/gtest.h>

#include "json/exception.h"
#include "nodes/array.h"
#include "nodes/boolean.h"
#include "nodes/node.h"
#include "nodes/null.h"
#include "nodes/number.h"
#include "nodes/object.h"
#include "nodes/string.h"

class StringVisitorTest : public ::testing::Test {};

TEST_F(StringVisitorTest, ArrayBadCast) {
  json::Array array;
  json::visitors::StringVisitor visitor;
  ASSERT_THROW(array.accept(visitor), json::BadCastException);
}

TEST_F(StringVisitorTest, BooleanBadCast) {
  json::Boolean boolean(true);
  json::visitors::StringVisitor visitor;
  ASSERT_THROW(boolean.accept(visitor), json::BadCastException);
}

TEST_F(StringVisitorTest, NullBadCast) {
  json::Null null;
  json::visitors::StringVisitor visitor;
  ASSERT_THROW(null.accept(visitor), json::BadCastException);
}

TEST_F(StringVisitorTest, NumberBadCast) {
  json::Number number(42.0);
  json::visitors::StringVisitor visitor;
  ASSERT_THROW(number.accept(visitor), json::BadCastException);
}

TEST_F(StringVisitorTest, ObjectBadCast) {
  json::Object object;
  json::visitors::StringVisitor visitor;
  ASSERT_THROW(object.accept(visitor), json::BadCastException);
}

TEST_F(StringVisitorTest, String) {
  json::String string("string");
  json::visitors::StringVisitor visitor;
  string.accept(visitor);
  ASSERT_EQ(visitor.result(), "string");
}
