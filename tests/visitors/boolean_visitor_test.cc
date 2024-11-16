#include "visitors/boolean_visitor.h"

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

class BooleanVisitorTest : public ::testing::Test {
 protected:
  void SetUp() override {
    json::utils::init_logging(boost::log::trivial::debug);
  }
};

TEST_F(BooleanVisitorTest, ArrayBadCast) {
  json::Array array;
  json::visitors::BooleanVisitor visitor;
  ASSERT_THROW(array.accept(visitor), json::BadCastException);
}

TEST_F(BooleanVisitorTest, Boolean) {
  json::Boolean boolean(true);
  json::visitors::BooleanVisitor visitor;
  boolean.accept(visitor);
  ASSERT_EQ(visitor.result(), true);
}

TEST_F(BooleanVisitorTest, NullBadCast) {
  json::Null null;
  json::visitors::BooleanVisitor visitor;
  ASSERT_THROW(null.accept(visitor), json::BadCastException);
}

TEST_F(BooleanVisitorTest, NumberBadCast) {
  json::Number number(42);
  json::visitors::BooleanVisitor visitor;
  ASSERT_THROW(number.accept(visitor), json::BadCastException);
}

TEST_F(BooleanVisitorTest, ObjectBadCast) {
  json::Object object;
  json::visitors::BooleanVisitor visitor;
  ASSERT_THROW(object.accept(visitor), json::BadCastException);
}

TEST_F(BooleanVisitorTest, StringBadCast) {
  json::String string("string");
  json::visitors::BooleanVisitor visitor;
  ASSERT_THROW(string.accept(visitor), json::BadCastException);
}
