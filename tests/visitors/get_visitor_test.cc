#include "visitors/get_visitor.h"

#include <gtest/gtest.h>

#include <boost/log/trivial.hpp>
#include <stdexcept>

#include "json/exception.h"
#include "nodes/array.h"
#include "nodes/boolean.h"
#include "nodes/node.h"
#include "nodes/null.h"
#include "nodes/number.h"
#include "nodes/object.h"
#include "nodes/string.h"
#include "utils/logger.h"

class GetVisitorTest : public ::testing::Test {
 protected:
  void SetUp() override {
    json::utils::init_logging(boost::log::trivial::debug);
    object_ = new json::Object();
    object_->insert("key", new json::String("value"));

    array_ = new json::Array();
    array_->push_back(new json::Number(1));
    array_->push_back(new json::Boolean(true));
    array_->push_back(new json::String("string"));
    array_->push_back(new json::Null());
  }

  void TearDown() override {
    delete object_;
    delete array_;
  }

  json::Object* object_;
  json::Array* array_;
};

TEST_F(GetVisitorTest, Object) {
  json::visitors::GetVisitor visitor("");
  object_->accept(visitor);
  ASSERT_EQ(*visitor.result(), *object_);
}

TEST_F(GetVisitorTest, ObjectKey) {
  json::visitors::GetVisitor visitor("key");
  object_->accept(visitor);
  ASSERT_EQ(*visitor.result(), *(new json::String("value")));
}

TEST_F(GetVisitorTest, CreateNewObject) {
  json::visitors::GetVisitor visitor("new_key");
  object_->accept(visitor);
  ASSERT_EQ(*visitor.result(), *(new json::Null()));
}

TEST_F(GetVisitorTest, ObjectBadAccesIndex) {
  json::visitors::GetVisitor visitor(0);
  ASSERT_THROW(object_->accept(visitor), json::BadAccessException);
}

TEST_F(GetVisitorTest, Array) {
  json::visitors::GetVisitor visitor;
  array_->accept(visitor);
  ASSERT_EQ(*visitor.result(), *array_);
}

TEST_F(GetVisitorTest, ArrayOutOfBounds) {
  json::visitors::GetVisitor visitor(99);
  ASSERT_THROW(array_->accept(visitor), std::out_of_range);
}

TEST_F(GetVisitorTest, ArrayInvalidIndex) {
  json::visitors::GetVisitor visitor("invalid");
  ASSERT_THROW(array_->accept(visitor), json::BadAccessException);
}

TEST_F(GetVisitorTest, Number) {
  json::visitors::GetVisitor visitor(0);
  array_->accept(visitor);
  ASSERT_EQ(*(visitor.result()), *(new json::Number(1)));
}

TEST_F(GetVisitorTest, NumberBadAccessKey) {
  json::Number number(42);
  json::visitors::GetVisitor visitor("anything");
  ASSERT_THROW(number.accept(visitor), json::BadAccessException);
}

TEST_F(GetVisitorTest, NumberBadAccessIndex) {
  json::Number number(42);
  json::visitors::GetVisitor visitor(0);
  ASSERT_THROW(number.accept(visitor), json::BadAccessException);
}

TEST_F(GetVisitorTest, Boolean) {
  json::visitors::GetVisitor visitor(1);
  array_->accept(visitor);
  ASSERT_EQ(*(visitor.result()), *(new json::Boolean(true)));
}

TEST_F(GetVisitorTest, BooleanBadAccessKey) {
  json::Boolean boolean(true);
  json::visitors::GetVisitor visitor("anything");
  ASSERT_THROW(boolean.accept(visitor), json::BadAccessException);
}

TEST_F(GetVisitorTest, BooleanBadAccessIndex) {
  json::Boolean boolean(true);
  json::visitors::GetVisitor visitor(0);
  ASSERT_THROW(boolean.accept(visitor), json::BadAccessException);
}

TEST_F(GetVisitorTest, String) {
  json::visitors::GetVisitor visitor(2);
  array_->accept(visitor);
  ASSERT_EQ(*(visitor.result()), *(new json::String("string")));
}

TEST_F(GetVisitorTest, StringBadAccessKey) {
  json::String str("test");
  json::visitors::GetVisitor visitor("anything");
  ASSERT_THROW(str.accept(visitor), json::BadAccessException);
}

TEST_F(GetVisitorTest, StringBadAccessIndex) {
  json::String str("test");
  json::visitors::GetVisitor visitor(0);
  ASSERT_THROW(str.accept(visitor), json::BadAccessException);
}

TEST_F(GetVisitorTest, Null) {
  json::visitors::GetVisitor visitor(3);
  array_->accept(visitor);
  ASSERT_EQ(*(visitor.result()), *(new json::Null()));
}

TEST_F(GetVisitorTest, NullBadAccessKey) {
  json::Null null;
  json::visitors::GetVisitor visitor("anything");
  ASSERT_THROW(null.accept(visitor), json::BadAccessException);
}

TEST_F(GetVisitorTest, NullBadAccessIndex) {
  json::Null null;
  json::visitors::GetVisitor visitor(0);
  ASSERT_THROW(null.accept(visitor), json::BadAccessException);
}
