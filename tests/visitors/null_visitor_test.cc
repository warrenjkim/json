#include "visitors/null_visitor.h"

#include <gtest/gtest.h>

#include "json/exception.h"
#include "nodes/array.h"
#include "nodes/boolean.h"
#include "nodes/node.h"
#include "nodes/null.h"
#include "nodes/number.h"
#include "nodes/object.h"
#include "nodes/string.h"

class NullVisitorTest : public ::testing::Test {};

TEST_F(NullVisitorTest, ArrayBadCast) {
  json::Array array;
  json::visitors::NullVisitor visitor;
  ASSERT_THROW(array.accept(visitor), json::BadCastException);
}

TEST_F(NullVisitorTest, BooleanBadCast) {
  json::Boolean boolean(true);
  json::visitors::NullVisitor visitor;
  ASSERT_THROW(boolean.accept(visitor), json::BadCastException);
}

TEST_F(NullVisitorTest, Null) {
  json::Null null;
  json::visitors::NullVisitor visitor;
  null.accept(visitor);
  ASSERT_EQ(visitor.result(), nullptr);
}

TEST_F(NullVisitorTest, NumberBadCast) {
  json::Number number(42);
  json::visitors::NullVisitor visitor;
  ASSERT_THROW(number.accept(visitor), json::BadCastException);
}

TEST_F(NullVisitorTest, ObjectBadCast) {
  json::Object object;
  json::visitors::NullVisitor visitor;
  ASSERT_THROW(object.accept(visitor), json::BadCastException);
}

TEST_F(NullVisitorTest, StringBadCast) {
  json::String string("string");
  json::visitors::NullVisitor visitor;
  ASSERT_THROW(string.accept(visitor), json::BadCastException);
}
