#include "visitors/set_visitor.h"

#include <gtest/gtest.h>

#include <boost/log/trivial.hpp>

#include "json/value.h"
#include "nodes/array.h"
#include "nodes/boolean.h"
#include "nodes/node.h"
#include "nodes/null.h"
#include "nodes/number.h"
#include "nodes/object.h"
#include "nodes/string.h"
#include "utils/logger.h"

class SetVisitorTest : public ::testing::Test {
 protected:
  void SetUp() override {
    json::utils::init_logging(boost::log::trivial::debug);
  }

  void TearDown() override {
    boost::log::core::get()->remove_all_sinks();
    original_ = nullptr;
  }

  json::Node* original_ = nullptr;
};

TEST_F(SetVisitorTest, SetBooleanValue) {
  original_ = new json::Boolean(false);
  json::Boolean new_value(true);

  json::visitors::SetVisitor visitor(original_);
  new_value.accept(visitor);

  ASSERT_TRUE(json::Value(original_));
}

TEST_F(SetVisitorTest, SetNumberValue) {
  original_ = new json::Number(42);
  json::Number new_value(99);

  json::visitors::SetVisitor visitor(original_);
  new_value.accept(visitor);

  ASSERT_EQ(json::Value(original_), 99);
}

TEST_F(SetVisitorTest, SetStringValue) {
  original_ = new json::String("hello");
  json::String new_value("world");

  json::visitors::SetVisitor visitor(original_);
  new_value.accept(visitor);

  ASSERT_EQ(json::Value(original_), "world");
}

TEST_F(SetVisitorTest, SetNullValue) {
  original_ = new json::Boolean(true);
  json::Null new_value;

  json::visitors::SetVisitor visitor(original_);
  new_value.accept(visitor);

  ASSERT_EQ(json::Value(original_), nullptr);
}

TEST_F(SetVisitorTest, SetObjectValue) {
  original_ = new json::Object();
  json::Object new_value;
  new_value.add("key", new json::Number(123));

  json::visitors::SetVisitor visitor(original_);
  new_value.accept(visitor);

  ASSERT_EQ(*original_, new_value);
}

TEST_F(SetVisitorTest, SetArrayValue) {
  original_ = new json::Array();
  json::Array new_value;
  new_value.add(new json::Number(123));

  json::visitors::SetVisitor visitor(original_);
  new_value.accept(visitor);

  ASSERT_EQ(*original_, new_value);
}

TEST_F(SetVisitorTest, SetArrayToString) {
  original_ = new json::Array({new json::Number(123)});
  json::String new_value("converted");

  json::visitors::SetVisitor visitor(original_);
  new_value.accept(visitor);

  ASSERT_EQ(json::Value(original_), "converted");
}

TEST_F(SetVisitorTest, SetBooleanToObject) {
  original_ = new json::Boolean(true);
  json::Object new_value;
  new_value.add("key", new json::Number(123));

  json::visitors::SetVisitor visitor(original_);
  new_value.accept(visitor);

  ASSERT_EQ(*original_, new_value);
}

TEST_F(SetVisitorTest, SetNullToArray) {
  original_ = new json::Null();
  json::Array new_value;
  new_value.add(new json::String("element"));

  json::visitors::SetVisitor visitor(original_);
  new_value.accept(visitor);

  ASSERT_EQ(*original_, new_value);
}

TEST_F(SetVisitorTest, SetNumberToBoolean) {
  original_ = new json::Number(123);
  json::Boolean new_value(false);

  json::visitors::SetVisitor visitor(original_);
  new_value.accept(visitor);

  ASSERT_FALSE(json::Value(original_));
}
