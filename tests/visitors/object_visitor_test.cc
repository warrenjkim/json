#include "visitors/object_visitor.h"

#include <gtest/gtest.h>

#include "json/exception.h"
#include "nodes/array.h"
#include "nodes/boolean.h"
#include "nodes/node.h"
#include "nodes/null.h"
#include "nodes/number.h"
#include "nodes/object.h"
#include "nodes/string.h"

class ObjectVisitorTest : public ::testing::Test {};

TEST_F(ObjectVisitorTest, ArrayBadCast) {
  json::Array array;
  json::visitors::ObjectVisitor visitor;
  ASSERT_THROW(array.accept(visitor), json::BadCastException);
}

TEST_F(ObjectVisitorTest, BooleanBadCast) {
  json::Boolean boolean(true);
  json::visitors::ObjectVisitor visitor;
  ASSERT_THROW(boolean.accept(visitor), json::BadCastException);
}

TEST_F(ObjectVisitorTest, NullBadCast) {
  json::Null null;
  json::visitors::ObjectVisitor visitor;
  ASSERT_THROW(null.accept(visitor), json::BadCastException);
}

TEST_F(ObjectVisitorTest, NumberBadCast) {
  json::Number number(42);
  json::visitors::ObjectVisitor visitor;
  ASSERT_THROW(number.accept(visitor), json::BadCastException);
}

TEST_F(ObjectVisitorTest, Object) {
  json::Object object;
  object.insert("null", new json::Null());
  object.insert("number", new json::Number(42));
  object.insert("boolean", new json::Boolean(true));
  object.insert("string", new json::String("string"));
  json::visitors::ObjectVisitor visitor;
  object.accept(visitor);
  ASSERT_EQ(visitor.result(), object.get());
}

TEST_F(ObjectVisitorTest, StringBadCast) {
  json::String string("string");
  json::visitors::ObjectVisitor visitor;
  ASSERT_THROW(string.accept(visitor), json::BadCastException);
}
