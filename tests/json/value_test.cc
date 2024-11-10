#include "json/value.h"

#include <gtest/gtest.h>

#include <boost/log/trivial.hpp>
#include <cstddef>
#include <string>

#include "nodes/array.h"
#include "nodes/boolean.h"
#include "nodes/node.h"
#include "nodes/null.h"
#include "nodes/number.h"
#include "nodes/object.h"
#include "nodes/string.h"
#include "utils/logger.h"

class ValueTest : public ::testing::Test {
 protected:
  void SetUp() override {
    json::utils::init_logging(boost::log::trivial::debug);
    root_ = new json::Object();
    root_->add("null", new json::Null());
    root_->add("bool", new json::Boolean(true));
    root_->add("number", new json::Number(42.5));
    root_->add("string", new json::String("hello"));

    array_ = new json::Array();
    array_->add(new json::Number(1));
    array_->add(new json::String("two"));
    array_->add(new json::Boolean(false));
    root_->add("array", array_);

    nested_obj_ = new json::Object();
    nested_obj_->add("key", new json::String("value"));
    root_->add("object", nested_obj_);
  }

  void TearDown() override { delete root_; }

  json::Object* root_;
  json::Array* array_;
  json::Object* nested_obj_;
};

TEST_F(ValueTest, ConvertNumberToInt) {
  json::Value value(new json::Number(42.5));
  int result = value;
  ASSERT_EQ(result, 42);
}

TEST_F(ValueTest, ConvertNumberToDouble) {
  json::Value value(new json::Number(42.5));
  double result = value;
  ASSERT_DOUBLE_EQ(result, 42.5);
}

TEST_F(ValueTest, ConvertNumberToFloat) {
  json::Value value(new json::Number(42.5));
  float result = value;
  ASSERT_FLOAT_EQ(result, 42.5f);
}

TEST_F(ValueTest, ConvertStringToStdString) {
  json::Value value(new json::String("test"));
  std::string result = value;
  ASSERT_EQ(result, "test");
}

TEST_F(ValueTest, ConvertStringToCString) {
  json::Value value(new json::String("test"));
  const char* result = value;
  ASSERT_STREQ(result, "test");
}

TEST_F(ValueTest, ConvertBooleanToBool) {
  json::Value value(new json::Boolean(true));
  bool result = value;
  ASSERT_TRUE(result);
}

TEST_F(ValueTest, ConvertBooleanToNullPtrT) {
  json::Value value(new json::Null());
  std::nullptr_t result = value;
  ASSERT_TRUE(result == nullptr);
}

TEST_F(ValueTest, AccessArrayByIndex) {
  json::Value value(array_);
  ASSERT_EQ(value[0], 1);
}

TEST_F(ValueTest, AccessArrayMixedTypes) {
  json::Value value(array_);
  ASSERT_EQ(value[0], 1);
  ASSERT_EQ(value[1], "two");
  ASSERT_EQ(value[2], false);
}

TEST_F(ValueTest, AccessObjectByKey) {
  json::Value value(nested_obj_);
  ASSERT_EQ(value["key"], "value");
}

TEST_F(ValueTest, CompareNumberToInt) {
  json::Value value(new json::Number(42));
  ASSERT_TRUE(value == 42);
  ASSERT_TRUE(42 == value);
}

TEST_F(ValueTest, CompareNumberToDouble) {
  json::Value value(new json::Number(42));
  ASSERT_TRUE(value == 42.0);
  ASSERT_TRUE(42.0 == value);
}

TEST_F(ValueTest, CompareStringToCString) {
  json::Value value(new json::String("test"));
  ASSERT_TRUE(value == "test");
  ASSERT_TRUE("test" == value);
}

TEST_F(ValueTest, CompareStringToStdString) {
  json::Value value(new json::String("test"));
  ASSERT_TRUE(value == std::string("test"));
  ASSERT_TRUE(std::string("test") == value);
}

TEST_F(ValueTest, CompareBooleanToBool) {
  json::Value value(new json::Boolean(true));
  ASSERT_TRUE(value == true);
  ASSERT_TRUE(true == value);
}

TEST_F(ValueTest, CompareValueToValueNumber) {
  json::Value value1(new json::Number(42));
  json::Value value2(new json::Number(42));
  ASSERT_TRUE(value1 == value2);
}

TEST_F(ValueTest, CompareValueToValueBool) {
  json::Value value1(new json::Boolean(true));
  json::Value value2(new json::Boolean(true));
  ASSERT_TRUE(value1 == value2);
}

TEST_F(ValueTest, CompareValueToValueArray) {
  json::Value value1(new json::Array());
  json::Value value2(new json::Array());
  ASSERT_TRUE(value1 == value2);
}

TEST_F(ValueTest, CompareValueToValueObject) {
  json::Value value1(new json::Object());
  json::Value value2(new json::Object());
  ASSERT_TRUE(value1 == value2);
}

TEST_F(ValueTest, CompareValueToValueString) {
  json::Value value1(new json::String("string"));
  json::Value value2(new json::String("string"));
  ASSERT_TRUE(value1 == value2);
}

TEST_F(ValueTest, CompareValueToValueNotSameType) {
  json::Value value1(new json::Number(42));
  json::Value value2(new json::String("string"));
  ASSERT_FALSE(value1 == value2);
}

TEST_F(ValueTest, CompareNullToNullptr) {
  json::Value value(new json::Null());
  ASSERT_TRUE(value == nullptr);
  ASSERT_TRUE(nullptr == value);
}

TEST_F(ValueTest, BadCastNumberToBool) {
  json::Value value(new json::Number(42));
  ASSERT_THROW(bool b = value, json::BadCastException);
}

TEST_F(ValueTest, BadCastBooleanToNumber) {
  json::Value value(new json::Boolean(true));
  ASSERT_THROW(int n = value, json::BadCastException);
}

TEST_F(ValueTest, BadCastBooleanToString) {
  json::Value value(new json::Boolean(true));
  ASSERT_THROW(std::string s = value, json::BadCastException);
  ASSERT_THROW(const char* cs = value, json::BadCastException);
}

TEST_F(ValueTest, BadCastBooleanToNullptr) {
  json::Value value(new json::Boolean(true));
  ASSERT_THROW(std::nullptr_t n = value, json::BadCastException);
}

TEST_F(ValueTest, BadCastNumberToString) {
  json::Value value(new json::Number(42));
  ASSERT_THROW(std::string s = value, json::BadCastException);
}

TEST_F(ValueTest, BadAccessNumberAsObject) {
  json::Value value(new json::Number(42));
  ASSERT_THROW(value["key"], json::BadAccessException);
}

TEST_F(ValueTest, BadAccessNumberAsArray) {
  json::Value value(new json::Number(42));
  ASSERT_THROW(value[0], json::BadAccessException);
}

TEST_F(ValueTest, BadCastStringToInt) {
  json::Value value(new json::String("test"));
  ASSERT_THROW(int i = value, json::BadCastException);
}

TEST_F(ValueTest, BadCastStringToBool) {
  json::Value value(new json::String("test"));
  ASSERT_THROW(bool b = value, json::BadCastException);
}

TEST_F(ValueTest, ChainedObjectAccess) {
  json::Value value(root_);
  ASSERT_EQ(std::string(value["object"]["key"]), "value");
}

TEST_F(ValueTest, ChainedArrayAccess) {
  json::Value value(root_);
  ASSERT_EQ(int(value["array"][0]), 1);
}

TEST_F(ValueTest, CompareNullToNull) {
  json::Value value(new json::Null());
  json::Value other(new json::Null());
  ASSERT_TRUE(value == other);
}

TEST_F(ValueTest, BadCastNullToInt) {
  json::Value value(new json::Null());
  ASSERT_THROW(int i = value, json::BadCastException);
}

TEST_F(ValueTest, BadCastNullToBool) {
  json::Value value(new json::Null());
  ASSERT_THROW(bool b = value, json::BadCastException);
}

TEST_F(ValueTest, BadCastNullToString) {
  json::Value value(new json::Null());
  ASSERT_THROW(std::string s = value, json::BadCastException);
}

TEST_F(ValueTest, BadCastNoValueSet) {
  json::Value value;
  ASSERT_THROW(std::string s = value[0], json::BadCastException);
  ASSERT_THROW(std::string s = value["key"], json::BadCastException);
}
