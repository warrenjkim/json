#include "visitors/array_visitor.h"

#include <gtest/gtest.h>

#include "nodes/array.h"
#include "nodes/boolean.h"
#include "nodes/node.h"
#include "nodes/null.h"
#include "nodes/number.h"
#include "nodes/object.h"
#include "nodes/string.h"
#include "utils/exception.h"

class ArrayVisitorTest : public ::testing::Test {};

TEST_F(ArrayVisitorTest, Array) {
  json::nodes::Array array;
  array.push_back(new json::nodes::Null());
  array.push_back(new json::nodes::Number(42));
  array.push_back(new json::nodes::Boolean(true));
  array.push_back(new json::nodes::String("string"));
  json::visitors::ArrayVisitor visitor;
  array.accept(visitor);
  ASSERT_EQ(visitor.result(), array.get());
}

TEST_F(ArrayVisitorTest, BooleanBadCast) {
  json::nodes::Boolean boolean(true);
  json::visitors::ArrayVisitor visitor;
  ASSERT_THROW(boolean.accept(visitor), json::BadCastException);
}

TEST_F(ArrayVisitorTest, NullBadCast) {
  json::nodes::Null null;
  json::visitors::ArrayVisitor visitor;
  ASSERT_THROW(null.accept(visitor), json::BadCastException);
}

TEST_F(ArrayVisitorTest, NumberBadCast) {
  json::nodes::Number number(42);
  json::visitors::ArrayVisitor visitor;
  ASSERT_THROW(number.accept(visitor), json::BadCastException);
}

TEST_F(ArrayVisitorTest, ObjectBadCast) {
  json::nodes::Object object;
  json::visitors::ArrayVisitor visitor;
  ASSERT_THROW(object.accept(visitor), json::BadCastException);
}

TEST_F(ArrayVisitorTest, StringBadCast) {
  json::nodes::String string("string");
  json::visitors::ArrayVisitor visitor;
  ASSERT_THROW(string.accept(visitor), json::BadCastException);
}
