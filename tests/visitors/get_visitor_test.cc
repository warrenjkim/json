#include "warren/internal/visitors/get_visitor.h"

#include "gtest/gtest.h"
#include "warren/internal/nodes/array.h"
#include "warren/internal/nodes/boolean.h"
#include "warren/internal/nodes/node.h"
#include "warren/internal/nodes/null.h"
#include "warren/internal/nodes/number.h"
#include "warren/internal/nodes/object.h"
#include "warren/internal/nodes/string.h"
#include "warren/json/exception.h"

class GetVisitorTest : public ::testing::Test {
 protected:
  void SetUp() override {
    object_.insert("key", new json::nodes::String("value"));

    array_.push_back(new json::nodes::Number(json::dsa::Numeric(1)));
    array_.push_back(new json::nodes::Boolean(true));
    array_.push_back(new json::nodes::String("string"));
    array_.push_back(new json::nodes::Null());
  }

  json::nodes::Object object_;
  json::nodes::Array array_;
};

TEST_F(GetVisitorTest, Object) {
  json::visitors::GetVisitor visitor("");
  object_.accept(visitor);
  EXPECT_EQ(*visitor.result(), object_);
}

TEST_F(GetVisitorTest, ObjectKey) {
  json::visitors::GetVisitor visitor("key");
  object_.accept(visitor);
  EXPECT_EQ(*visitor.result(), json::nodes::String("value"));
}

TEST_F(GetVisitorTest, CreateNewObject) {
  json::visitors::GetVisitor visitor("new_key");
  object_.accept(visitor);
  EXPECT_EQ(*visitor.result(), json::nodes::Null());
}

TEST_F(GetVisitorTest, ObjectBadAccesIndex) {
  json::visitors::GetVisitor visitor(0);
  EXPECT_THROW(object_.accept(visitor), json::BadAccessException);
}

TEST_F(GetVisitorTest, Array) {
  json::visitors::GetVisitor visitor;
  array_.accept(visitor);
  EXPECT_EQ(*visitor.result(), array_);
}

TEST_F(GetVisitorTest, ArrayOutOfBounds) {
  json::visitors::GetVisitor visitor(99);
  EXPECT_THROW(array_.accept(visitor), std::out_of_range);
}

TEST_F(GetVisitorTest, ArrayInvalidIndex) {
  json::visitors::GetVisitor visitor("invalid");
  EXPECT_THROW(array_.accept(visitor), json::BadAccessException);
}

TEST_F(GetVisitorTest, Number) {
  json::visitors::GetVisitor visitor(0);
  array_.accept(visitor);
  EXPECT_EQ(*(visitor.result()), json::nodes::Number(json::dsa::Numeric(1)));
}

TEST_F(GetVisitorTest, NumberBadAccessKey) {
  json::nodes::Number number(42);
  json::visitors::GetVisitor visitor("anything");
  EXPECT_THROW(number.accept(visitor), json::BadAccessException);
}

TEST_F(GetVisitorTest, NumberBadAccessIndex) {
  json::nodes::Number number(42);
  json::visitors::GetVisitor visitor(0);
  EXPECT_THROW(number.accept(visitor), json::BadAccessException);
}

TEST_F(GetVisitorTest, Boolean) {
  json::visitors::GetVisitor visitor(1);
  array_.accept(visitor);
  EXPECT_EQ(*(visitor.result()), json::nodes::Boolean(true));
}

TEST_F(GetVisitorTest, BooleanBadAccessKey) {
  json::nodes::Boolean boolean(true);
  json::visitors::GetVisitor visitor("anything");
  EXPECT_THROW(boolean.accept(visitor), json::BadAccessException);
}

TEST_F(GetVisitorTest, BooleanBadAccessIndex) {
  json::nodes::Boolean boolean(true);
  json::visitors::GetVisitor visitor(0);
  EXPECT_THROW(boolean.accept(visitor), json::BadAccessException);
}

TEST_F(GetVisitorTest, String) {
  json::visitors::GetVisitor visitor(2);
  array_.accept(visitor);
  EXPECT_EQ(*(visitor.result()), json::nodes::String("string"));
}

TEST_F(GetVisitorTest, StringBadAccessKey) {
  json::nodes::String str("test");
  json::visitors::GetVisitor visitor("anything");
  EXPECT_THROW(str.accept(visitor), json::BadAccessException);
}

TEST_F(GetVisitorTest, StringBadAccessIndex) {
  json::nodes::String str("test");
  json::visitors::GetVisitor visitor(0);
  EXPECT_THROW(str.accept(visitor), json::BadAccessException);
}

TEST_F(GetVisitorTest, Null) {
  json::visitors::GetVisitor visitor(3);
  array_.accept(visitor);
  EXPECT_EQ(*(visitor.result()), json::nodes::Null());
}

TEST_F(GetVisitorTest, NullBadAccessKey) {
  json::nodes::Null null;
  json::visitors::GetVisitor visitor("anything");
  EXPECT_THROW(null.accept(visitor), json::BadAccessException);
}

TEST_F(GetVisitorTest, NullBadAccessIndex) {
  json::nodes::Null null;
  json::visitors::GetVisitor visitor(0);
  EXPECT_THROW(null.accept(visitor), json::BadAccessException);
}
