#include "visitors/array_visitor.h"

#include <gtest/gtest.h>

#include <boost/log/trivial.hpp>

#include "json/exception.h"
#include "nodes/array.h"
#include "nodes/boolean.h"
#include "nodes/node.h"
#include "nodes/null.h"
#include "nodes/number.h"
#include "nodes/object.h"
#include "nodes/string.h"
#include "utils/logger.h"

class ArrayVisitorTest : public ::testing::Test {
 protected:
  void SetUp() override {
    json::utils::init_logging(boost::log::trivial::debug);
  }
};

TEST_F(ArrayVisitorTest, Array) {
  json::Array array;
  array.add(new json::Null());
  array.add(new json::Number(42));
  array.add(new json::Boolean(true));
  array.add(new json::String("string"));
  json::visitors::ArrayVisitor visitor;
  array.accept(visitor);
  ASSERT_EQ(visitor.result(), array.get());
}

TEST_F(ArrayVisitorTest, BooleanBadCast) {
  json::Boolean boolean(true);
  json::visitors::ArrayVisitor visitor;
  ASSERT_THROW(boolean.accept(visitor), json::BadCastException);
}

TEST_F(ArrayVisitorTest, NullBadCast) {
  json::Null null;
  json::visitors::ArrayVisitor visitor;
  ASSERT_THROW(null.accept(visitor), json::BadCastException);
}

TEST_F(ArrayVisitorTest, NumberBadCast) {
  json::Number number(42);
  json::visitors::ArrayVisitor visitor;
  ASSERT_THROW(number.accept(visitor), json::BadCastException);
}

TEST_F(ArrayVisitorTest, ObjectBadCast) {
  json::Object object;
  json::visitors::ArrayVisitor visitor;
  ASSERT_THROW(object.accept(visitor), json::BadCastException);
}

TEST_F(ArrayVisitorTest, StringBadCast) {
  json::String string("string");
  json::visitors::ArrayVisitor visitor;
  ASSERT_THROW(string.accept(visitor), json::BadCastException);
}
