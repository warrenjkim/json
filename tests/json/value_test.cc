#include "json/value.h"

#include <gtest/gtest.h>

#include <boost/log/trivial.hpp>
#include <cstddef>
#include <string>

#include "json/exception.h"
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
    array_->add(new json::Null());
    root_->add("array", array_);

    nested_obj_ = new json::Object();
    nested_obj_->add("key", new json::String("value"));
    root_->add("object", nested_obj_);
  }

  void TearDown() override {}

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
  nullptr_t result = value;
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
  ASSERT_THROW(nullptr_t n = value, json::BadCastException);
}

TEST_F(ValueTest, BadCastNumberToString) {
  json::Value value(new json::Number(42));
  ASSERT_THROW(std::string s = value, json::BadCastException);
}

TEST_F(ValueTest, BadAccessNumberAsObject) {
  json::Value value(new json::Number(42));
  ASSERT_THROW((void)value["key"], json::BadAccessException);
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
  ASSERT_EQ(value["object"]["key"], "value");
}

TEST_F(ValueTest, ChainedArrayAccess) {
  json::Value value(root_);
  ASSERT_EQ(value["array"][0], 1);
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
}

TEST_F(ValueTest, UpdateArrayElement) {
  json::Value value(array_);
  ASSERT_EQ(value[3], nullptr);
  value[3] = 4;
  ASSERT_EQ(value[3], 4);
}

TEST_F(ValueTest, UpdateObjectValue) {
  json::Value value(nested_obj_);
  ASSERT_EQ(value["key"], "value");
  value["key"] = "new_value";
  ASSERT_EQ(value["key"], "new_value");
}

TEST_F(ValueTest, AddNewObjectKey) {
  json::Value value(nested_obj_);
  value["new_key"] = 42;
  ASSERT_EQ(value["new_key"], 42);
}

TEST_F(ValueTest, UpdateArrayWithMixedTypes) {
  json::Value value(array_);
  value[0] = "string";
  value[1] = 42;
  value[2] = nullptr;

  ASSERT_EQ(value[0], "string");
  ASSERT_EQ(value[1], 42);
  ASSERT_EQ(value[2], nullptr);
}

TEST_F(ValueTest, UpdateNestedObjectValue) {
  json::Value value(root_);
  ASSERT_EQ(value["object"]["key"], "value");
  value["object"]["key"] = "updated";
  ASSERT_EQ(value["object"]["key"], "updated");
}

TEST_F(ValueTest, UpdatePrimitiveTypes) {
  json::Value number_val(new json::Number(42));
  json::Value string_val(new json::String("test"));
  json::Value bool_val(new json::Boolean(false));

  number_val = 100;
  string_val = "updated";
  bool_val = true;

  ASSERT_EQ(number_val, 100);
  ASSERT_EQ(string_val, "updated");
  ASSERT_EQ(bool_val, true);
}

TEST_F(ValueTest, AddToEmptyObject) {
  json::Value value;
  ASSERT_EQ(value["key"], json::Object());
  value["key"] = 10;
  ASSERT_EQ(value["key"], 10);
}

TEST_F(ValueTest, AddToEmptyArrayNumberFirst) {
  json::Array array;
  array.add(new json::Number(10));
  array.add(new json::Boolean(true));

  json::Value value;
  value.add(10);
  value.add(true);

  ASSERT_EQ(value, array);
  ASSERT_EQ(array, value);
}

TEST_F(ValueTest, AddToEmptyArrayBooleanFirst) {
  json::Array array;
  array.add(new json::Boolean(true));
  array.add(new json::Number(10));

  json::Value value;
  value.add(true);
  value.add(10);

  ASSERT_EQ(value, array);
  ASSERT_EQ(array, value);
}

TEST_F(ValueTest, AddToEmptyArrayNullFirst) {
  json::Array array;
  array.add(new json::Null());
  array.add(new json::Boolean(true));

  json::Value value;
  value.add(nullptr);
  value.add(true);

  ASSERT_EQ(value, array);
  ASSERT_EQ(array, value);
}

TEST_F(ValueTest, AddToEmptyArrayCStringFirst) {
  json::Array array;
  array.add(new json::String("string"));
  array.add(new json::Null());

  json::Value value;
  value.add("string");
  value.add(nullptr);

  ASSERT_EQ(value, array);
  ASSERT_EQ(array, value);
}

TEST_F(ValueTest, AddToEmptyArrayStringFirst) {
  json::Array array;
  array.add(new json::String("string"));
  array.add(new json::Null());

  json::Value value;
  value.add(std::string("string"));
  value.add(nullptr);

  ASSERT_EQ(value, array);
  ASSERT_EQ(array, value);
}

TEST_F(ValueTest, AddToEmptyArrayValueFirst) {
  json::Value num(new json::Number(8));

  json::Array array;
  array.add(new json::Number(8));
  array.add(new json::String("string"));

  json::Value value;
  value.add(num);
  value.add("string");

  ASSERT_EQ(value, array);
  ASSERT_EQ(array, value);
}

TEST_F(ValueTest, AddToEmptyArrayEmptyValueFirst) {
  json::Value null_value;

  json::Array array;
  array.add(new json::Null());
  array.add(new json::String("string"));

  json::Value value;
  value.add(null_value);
  value.add("string");

  ASSERT_EQ(value, array);
  ASSERT_EQ(array, value);
}

TEST_F(ValueTest, AddToEmptyObjectNumberFirst) {
  json::Object object;
  object.add("num", new json::Number(10));
  object.add("bool", new json::Boolean(true));

  json::Value value;
  value.put("num", 10);
  value.put("bool", true);

  ASSERT_EQ(value, object);
  ASSERT_EQ(object, value);
}

TEST_F(ValueTest, AddToEmptyObjectBooleanFirst) {
  json::Object object;
  object.add("bool", new json::Boolean(true));
  object.add("num", new json::Number(10));

  json::Value value;
  value.put("bool", true);
  value.put("num", 10);

  ASSERT_EQ(value, object);
  ASSERT_EQ(object, value);
}

TEST_F(ValueTest, AddToEmptyObjectNullFirst) {
  json::Object object;
  object.add("null", new json::Null());
  object.add("bool", new json::Boolean(true));

  json::Value value;
  value.put("null", nullptr);
  value.put("bool", true);

  ASSERT_EQ(value, object);
  ASSERT_EQ(object, value);
}

TEST_F(ValueTest, AddToEmptyObjectCStringFirst) {
  json::Object object;
  object.add("str", new json::String("string"));
  object.add("null", new json::Null());

  json::Value value;
  value.put("str", "string");
  value.put("null", nullptr);

  ASSERT_EQ(value, object);
  ASSERT_EQ(object, value);
}

TEST_F(ValueTest, AddToEmptyObjectStringFirst) {
  json::Object object;
  object.add("str", new json::String("string"));
  object.add("null", new json::Null());

  json::Value value;
  value.put("str", std::string("string"));
  value.put("null", nullptr);

  ASSERT_EQ(value, object);
  ASSERT_EQ(object, value);
}

TEST_F(ValueTest, AddToEmptyObjectValueFirst) {
  json::Value num(new json::Number(8));

  json::Object object;
  object.add("val", new json::Number(8));
  object.add("str", new json::String("string"));

  json::Value value;
  value.put("val", num);
  value.put("str", "string");

  ASSERT_EQ(value, object);
  ASSERT_EQ(object, value);
}

TEST_F(ValueTest, AddToEmptyObjectEmptyValueFirst) {
  json::Value null_value;

  json::Object object;
  object.add("empty_val", new json::Null());
  object.add("str", new json::String("string"));

  json::Value value;
  value.put("empty_val", null_value);
  value.put("str", "string");

  ASSERT_EQ(value, object);
  ASSERT_EQ(object, value);
}

TEST_F(ValueTest, ValueAssignment) {
  json::Value bool_value = true;
  json::Value number_value = 42;
  json::Value null_value = nullptr;
  json::Value cstring_value = "some_cstring";
  json::Value string_value = std::string("some_string");

  json::Value arr;
  arr.add(1);
  arr.add("2");

  json::Value json = "str";
  json = arr;

  ASSERT_EQ(bool_value, true);
  ASSERT_EQ(number_value, 42);
  ASSERT_EQ(null_value, nullptr);
  ASSERT_EQ(cstring_value, "some_cstring");
  ASSERT_EQ(string_value, "some_string");
  ASSERT_EQ(json, arr);
}

TEST_F(ValueTest, MoveConstructor) {
  json::Value original;
  original["key"] = "value";
  ASSERT_EQ(original["key"], "value");

  json::Value moved = std::move(original);

  ASSERT_EQ(moved["key"], "value");
  ASSERT_EQ(original["key"], json::Object());
}

TEST_F(ValueTest, MoveCleansUpDestination) {
  json::Value source;
  source.put("new", "content");

  json::Value dest;
  dest.put("old", "content");

  dest = std::move(source);

  ASSERT_EQ(dest["new"], "content");
  ASSERT_EQ(dest["old"], json::Object());
  ASSERT_EQ(source["new"], json::Object());
}

TEST_F(ValueTest, MoveSelfAssignmentNoOp) {
  json::Value val;
  val.put("test", "data");

  val = std::move(val);

  ASSERT_EQ(val["test"], "data");
}

TEST_F(ValueTest, ChangesPropagateToCache) {
  json::Value val;
  val["key"] = 10;
  val["key"] = "10";
  ASSERT_EQ(val["key"], "10");
}

TEST_F(ValueTest, AssignNull) {
  json::Value val(new json::Number(42));
  val = nullptr;
  ASSERT_EQ(val, nullptr);
}

TEST_F(ValueTest, CopyConstructor) {
  json::Value val(new json::Number(42));
  json::Value val_2(val);
  ASSERT_EQ(val, val_2);
}
