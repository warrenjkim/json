#include "visitors/number_visitor.h"

#include <gtest/gtest.h>

#include "utils/exception.h"
#include "nodes/array.h"
#include "nodes/boolean.h"
#include "nodes/node.h"
#include "nodes/null.h"
#include "nodes/number.h"
#include "nodes/object.h"
#include "nodes/string.h"

class NumberVisitorTest : public ::testing::Test {};

TEST_F(NumberVisitorTest, ArrayBadCast) {
  json::nodes::Array array;
  json::visitors::NumberVisitor visitor;
  ASSERT_THROW(array.accept(visitor), json::BadCastException);
}

TEST_F(NumberVisitorTest, BooleanBadCast) {
  json::nodes::Boolean boolean(true);
  json::visitors::NumberVisitor visitor;
  ASSERT_THROW(boolean.accept(visitor), json::BadCastException);
}

TEST_F(NumberVisitorTest, NullBadCast) {
  json::nodes::Null null;
  json::visitors::NumberVisitor visitor;
  ASSERT_THROW(null.accept(visitor), json::BadCastException);
}

TEST_F(NumberVisitorTest, Number) {
  json::nodes::Number number(42);
  json::visitors::NumberVisitor visitor;
  number.accept(visitor);
  ASSERT_EQ(visitor.result(), 42);
}

TEST_F(NumberVisitorTest, ObjectBadCast) {
  json::nodes::Object object;
  json::visitors::NumberVisitor visitor;
  ASSERT_THROW(object.accept(visitor), json::BadCastException);
}

TEST_F(NumberVisitorTest, StringBadCast) {
  json::nodes::String string("string");
  json::visitors::NumberVisitor visitor;
  ASSERT_THROW(string.accept(visitor), json::BadCastException);
}
