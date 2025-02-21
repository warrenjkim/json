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
    root_->insert("null", new json::Null());
    root_->insert("bool", new json::Boolean(true));
    root_->insert("number", new json::Number(42.5));
    root_->insert("string", new json::String("hello"));

    array_ = new json::Array();
    array_->push_back(new json::Number(1));
    array_->push_back(new json::String("two"));
    array_->push_back(new json::Boolean(false));
    array_->push_back(new json::Null());
    root_->insert("array", array_->clone());

    nested_obj_ = new json::Object();
    nested_obj_->insert("key", new json::String("value"));
    root_->insert("object", nested_obj_->clone());
  }

  void TearDown() override {}

  json::Object* root_;
  json::Array* array_;
  json::Object* nested_obj_;
};

TEST_F(ValueTest, ConvertNumberToInt) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value(new json::Number(42.5));
  int result = value;
  ASSERT_EQ(result, 42);
}

TEST_F(ValueTest, ConvertNumberToDouble) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value(new json::Number(42.5));
  double result = value;
  ASSERT_DOUBLE_EQ(result, 42.5);
}

TEST_F(ValueTest, ConvertNumberToFloat) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value(new json::Number(42.5));
  float result = value;
  ASSERT_FLOAT_EQ(result, 42.5f);
}

TEST_F(ValueTest, ConvertStringToStdString) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value(new json::String("test"));
  std::string result = value;
  ASSERT_EQ(result, "test");
}

TEST_F(ValueTest, ConvertStringToCString) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value(new json::String("test"));
  const char* result = value;
  ASSERT_STREQ(result, "test");
}

TEST_F(ValueTest, ConvertBooleanToBool) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value(new json::Boolean(true));
  bool result = value;
  ASSERT_TRUE(result);
}

TEST_F(ValueTest, ConvertBooleanToNullPtrT) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value(new json::Null());
  nullptr_t result = value;
  ASSERT_TRUE(result == nullptr);
}

TEST_F(ValueTest, AccessArrayByIndex) {
  delete root_;
  delete nested_obj_;

  json::Value value(array_);
  ASSERT_EQ(value[0], 1);
}

TEST_F(ValueTest, AccessArrayMixedTypes) {
  delete root_;
  delete nested_obj_;

  json::Value value(array_);
  ASSERT_EQ(value[0], 1);
  ASSERT_EQ(value[1], "two");
  ASSERT_EQ(value[2], false);
}

TEST_F(ValueTest, AccessObjectByKey) {
  delete root_;
  delete array_;

  json::Value value(nested_obj_);
  ASSERT_EQ(value["key"], "value");
}

TEST_F(ValueTest, CompareNumberToInt) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value(new json::Number(42));
  ASSERT_TRUE(value == 42);
  ASSERT_TRUE(42 == value);
}

TEST_F(ValueTest, CompareNumberToDouble) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value(new json::Number(42));
  ASSERT_TRUE(value == 42.0);
  ASSERT_TRUE(42.0 == value);
}

TEST_F(ValueTest, CompareStringToCString) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value(new json::String("test"));
  ASSERT_TRUE(value == "test");
  ASSERT_TRUE("test" == value);
}

TEST_F(ValueTest, CompareStringToStdString) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value(new json::String("test"));
  ASSERT_TRUE(value == std::string("test"));
  ASSERT_TRUE(std::string("test") == value);
}

TEST_F(ValueTest, CompareBooleanToBool) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value(new json::Boolean(true));
  ASSERT_TRUE(value == true);
  ASSERT_TRUE(true == value);
}

TEST_F(ValueTest, CompareValueToValueNumber) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value1(new json::Number(42));
  json::Value value2(new json::Number(42));
  ASSERT_TRUE(value1 == value2);
}

TEST_F(ValueTest, CompareValueToValueBool) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value1(new json::Boolean(true));
  json::Value value2(new json::Boolean(true));
  ASSERT_TRUE(value1 == value2);
}

TEST_F(ValueTest, CompareValueToValueArray) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value1(new json::Array());
  json::Value value2(new json::Array());
  ASSERT_TRUE(value1 == value2);
}

TEST_F(ValueTest, CompareValueToValueObject) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value1(new json::Object());
  json::Value value2(new json::Object());
  ASSERT_TRUE(value1 == value2);
}

TEST_F(ValueTest, CompareValueToValueString) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value1(new json::String("string"));
  json::Value value2(new json::String("string"));
  ASSERT_TRUE(value1 == value2);
}

TEST_F(ValueTest, CompareValueToValueNotSameType) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value1(new json::Number(42));
  json::Value value2(new json::String("string"));
  ASSERT_FALSE(value1 == value2);
}

TEST_F(ValueTest, CompareNullToNullptr) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value(new json::Null());
  ASSERT_TRUE(value == nullptr);
  ASSERT_TRUE(nullptr == value);
}

TEST_F(ValueTest, BadCastNumberToBool) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value(new json::Number(42));
  ASSERT_THROW(bool b = value, json::BadCastException);
}

TEST_F(ValueTest, BadCastBooleanToNumber) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value(new json::Boolean(true));
  ASSERT_THROW(int n = value, json::BadCastException);
}

TEST_F(ValueTest, BadCastBooleanToString) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value(new json::Boolean(true));
  ASSERT_THROW(std::string s = value, json::BadCastException);
  ASSERT_THROW(const char* cs = value, json::BadCastException);
}

TEST_F(ValueTest, BadCastBooleanToNullptr) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value(new json::Boolean(true));
  ASSERT_THROW(nullptr_t n = value, json::BadCastException);
}

TEST_F(ValueTest, BadCastNumberToString) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value(new json::Number(42));
  ASSERT_THROW(std::string s = value, json::BadCastException);
}

TEST_F(ValueTest, BadAccessNumberAsObject) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value(new json::Number(42));
  ASSERT_THROW((void)value["key"], json::BadAccessException);
}

TEST_F(ValueTest, BadAccessNumberAsArray) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value(new json::Number(42));
  ASSERT_THROW(value[0], json::BadAccessException);
}

TEST_F(ValueTest, BadCastStringToInt) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value(new json::String("test"));
  ASSERT_THROW(int i = value, json::BadCastException);
}

TEST_F(ValueTest, BadCastStringToBool) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value(new json::String("test"));
  ASSERT_THROW(bool b = value, json::BadCastException);
}

TEST_F(ValueTest, ChainedObjectAccess) {
  delete array_;
  delete nested_obj_;

  json::Value value(root_);
  ASSERT_EQ(value["object"]["key"], "value");
}

TEST_F(ValueTest, ChainedArrayAccess) {
  delete array_;
  delete nested_obj_;

  json::Value value(root_);
  ASSERT_EQ(value["array"][0], 1);
}

TEST_F(ValueTest, CompareNullToNull) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value(new json::Null());
  json::Value other(new json::Null());
  ASSERT_TRUE(value == other);
}

TEST_F(ValueTest, BadCastNullToInt) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value(new json::Null());
  ASSERT_THROW(int i = value, json::BadCastException);
}

TEST_F(ValueTest, BadCastNullToBool) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value(new json::Null());
  ASSERT_THROW(bool b = value, json::BadCastException);
}

TEST_F(ValueTest, BadCastNullToString) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value(new json::Null());
  ASSERT_THROW(std::string s = value, json::BadCastException);
}

TEST_F(ValueTest, BadCastNoValueSet) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value;
  ASSERT_THROW(std::string s = value[0], json::BadCastException);
}

TEST_F(ValueTest, UpdateArrayElement) {
  delete root_;
  delete nested_obj_;

  json::Value value(array_);
  ASSERT_EQ(value[3], nullptr);
  value[3] = 4;
  ASSERT_EQ(value[3], 4);
}

TEST_F(ValueTest, UpdateObjectValue) {
  delete root_;
  delete array_;

  json::Value value(nested_obj_);
  ASSERT_EQ(value["key"], "value");
  value["key"] = "new_value";
  ASSERT_EQ(value["key"], "new_value");
}

TEST_F(ValueTest, AddNewObjectKey) {
  delete root_;
  delete array_;

  json::Value value(nested_obj_);
  value["new_key"] = 42;
  ASSERT_EQ(value["new_key"], 42);
}

TEST_F(ValueTest, UpdateArrayWithMixedTypes) {
  delete root_;
  delete nested_obj_;

  json::Value value(array_);
  value[0] = "string";
  value[1] = 42;
  value[2] = nullptr;

  ASSERT_EQ(value[0], "string");
  ASSERT_EQ(value[1], 42);
  ASSERT_EQ(value[2], nullptr);
}

TEST_F(ValueTest, UpdateNestedObjectValue) {
  delete array_;
  delete nested_obj_;

  json::Value value(root_);
  ASSERT_EQ(value["object"]["key"], "value");
  value["object"]["key"] = "updated";
  ASSERT_EQ(value["object"]["key"], "updated");
}

TEST_F(ValueTest, UpdatePrimitiveTypes) {
  delete root_;
  delete array_;
  delete nested_obj_;

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
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value;
  ASSERT_EQ(value["key"], nullptr);
  value["key"] = 10;
  ASSERT_EQ(value["key"], 10);
}

TEST_F(ValueTest, AddToEmptyArrayNumberFirst) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Array array;
  array.push_back(new json::Number(10));
  array.push_back(new json::Boolean(true));

  json::Value value;
  value.push_back(10);
  value.push_back(true);

  ASSERT_EQ(value, array);
  ASSERT_EQ(array, value);
}

TEST_F(ValueTest, AddToEmptyArrayBooleanFirst) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Array array;
  array.push_back(new json::Boolean(true));
  array.push_back(new json::Number(10));

  json::Value value;
  value.push_back(true);
  value.push_back(10);

  ASSERT_EQ(value, array);
  ASSERT_EQ(array, value);
}

TEST_F(ValueTest, AddToEmptyArrayNullFirst) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Array array;
  array.push_back(new json::Null());
  array.push_back(new json::Boolean(true));

  json::Value value;
  value.push_back(nullptr);
  value.push_back(true);

  ASSERT_EQ(value, array);
  ASSERT_EQ(array, value);
}

TEST_F(ValueTest, AddToEmptyArrayCStringFirst) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Array array;
  array.push_back(new json::String("string"));
  array.push_back(new json::Null());

  json::Value value;
  value.push_back("string");
  value.push_back(nullptr);

  ASSERT_EQ(value, array);
  ASSERT_EQ(array, value);
}

TEST_F(ValueTest, AddToEmptyArrayStringFirst) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Array array;
  array.push_back(new json::String("string"));
  array.push_back(new json::Null());

  json::Value value;
  value.push_back(std::string("string"));
  value.push_back(nullptr);

  ASSERT_EQ(value, array);
  ASSERT_EQ(array, value);
}

TEST_F(ValueTest, AddToEmptyArrayValueFirst) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value num(new json::Number(8));

  json::Array array;
  array.push_back(new json::Number(8));
  array.push_back(new json::String("string"));

  json::Value value;
  value.push_back(num);
  value.push_back("string");

  ASSERT_EQ(value, array);
  ASSERT_EQ(array, value);
}

TEST_F(ValueTest, AddToEmptyArrayEmptyValueFirst) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value null_value;

  json::Array array;
  array.push_back(new json::Null());
  array.push_back(new json::String("string"));

  json::Value value;
  value.push_back(null_value);
  value.push_back("string");

  ASSERT_EQ(value, array);
  ASSERT_EQ(array, value);
}

TEST_F(ValueTest, AddToEmptyObjectNumberFirst) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Object object;
  object.insert("num", new json::Number(10));
  object.insert("bool", new json::Boolean(true));

  json::Value value;
  value.insert("num", 10);
  value.insert("bool", true);

  ASSERT_EQ(value, object);
  ASSERT_EQ(object, value);
}

TEST_F(ValueTest, AddToEmptyObjectBooleanFirst) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Object object;
  object.insert("bool", new json::Boolean(true));
  object.insert("num", new json::Number(10));

  json::Value value;
  value.insert("bool", true);
  value.insert("num", 10);

  ASSERT_EQ(value, object);
  ASSERT_EQ(object, value);
}

TEST_F(ValueTest, AddToEmptyObjectNullFirst) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Object object;
  object.insert("null", new json::Null());
  object.insert("bool", new json::Boolean(true));

  json::Value value;
  value.insert("null", nullptr);
  value.insert("bool", true);

  ASSERT_EQ(value, object);
  ASSERT_EQ(object, value);
}

TEST_F(ValueTest, AddToEmptyObjectCStringFirst) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Object object;
  object.insert("str", new json::String("string"));
  object.insert("null", new json::Null());

  json::Value value;
  value.insert("str", "string");
  value.insert("null", nullptr);

  ASSERT_EQ(value, object);
  ASSERT_EQ(object, value);
}

TEST_F(ValueTest, AddToEmptyObjectStringFirst) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Object object;
  object.insert("str", new json::String("string"));
  object.insert("null", new json::Null());

  json::Value value;
  value.insert("str", std::string("string"));
  value.insert("null", nullptr);

  ASSERT_EQ(value, object);
  ASSERT_EQ(object, value);
}

TEST_F(ValueTest, AddToEmptyObjectValueFirst) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value num(new json::Number(8));

  json::Object object;
  object.insert("val", new json::Number(8));
  object.insert("str", new json::String("string"));

  json::Value value;
  value.insert("val", num);
  value.insert("str", "string");

  ASSERT_EQ(value, object);
  ASSERT_EQ(object, value);
}

TEST_F(ValueTest, AddToEmptyObjectEmptyValueFirst) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value null_value;

  json::Object object;
  object.insert("empty_val", new json::Null());
  object.insert("str", new json::String("string"));

  json::Value value;
  value.insert("empty_val", null_value);
  value.insert("str", "string");

  ASSERT_EQ(value, object);
  ASSERT_EQ(object, value);
}

TEST_F(ValueTest, ValueAssignment) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value bool_value = true;
  json::Value number_value = 42;
  json::Value null_value = nullptr;
  json::Value cstring_value = "some_cstring";
  json::Value string_value = std::string("some_string");

  ASSERT_EQ(bool_value, true);
  ASSERT_EQ(number_value, 42);
  ASSERT_EQ(null_value, nullptr);
  ASSERT_EQ(cstring_value, "some_cstring");
  ASSERT_EQ(string_value, "some_string");
}

TEST_F(ValueTest, ObjectChangesPropagateToCache) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value val;
  val["key"] = 10;
  ASSERT_EQ(val["key"], 10);

  val["key"] = "10";
  ASSERT_EQ(val["key"], "10");

  val["key"] = false;
  ASSERT_EQ(val["key"], false);
}

TEST_F(ValueTest, ArrayChangesPropagateToCache) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value val;
  val.push_back(10);
  ASSERT_EQ(val[0], 10);

  val[0] = "20";
  ASSERT_EQ(val[0], "20");

  val[0] = std::string("30");
  ASSERT_EQ(val[0], "30");

  val[0] = false;
  ASSERT_EQ(val[0], false);

  val[0] = 20;
  ASSERT_EQ(val[0], 20);
}

TEST_F(ValueTest, AssignNull) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value val(new json::Number(42));
  val = nullptr;
  ASSERT_EQ(val, nullptr);
}

TEST_F(ValueTest, CopyConstructor) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value val(new json::Number(42));
  json::Value val_2(val);
  ASSERT_EQ(val, val_2);
}

TEST_F(ValueTest, ObjectMoveSemantics) {
  delete root_;
  delete array_;
  delete nested_obj_;

  // arrange
  json::Value obj1;
  obj1["key1"] = "value1";
  obj1["key2"] = "value2";

  json::Value obj2;
  obj2["keyA"] = "valueA";

  // act
  obj2["keyX"] = std::move(obj1["key1"]);

  // assert
  json::Object expected_obj1;
  expected_obj1.get().insert("key2", new json::String("value2"));

  json::Object expected_obj2;
  expected_obj2.get().insert("keyA", new json::String("valueA"));
  expected_obj2.get().insert("keyX", new json::String("value1"));

  ASSERT_EQ(obj1, expected_obj1);
  ASSERT_EQ(obj2, expected_obj2);
}

TEST_F(ValueTest, ArrayMoveSemantics) {
  delete root_;
  delete array_;
  delete nested_obj_;

  // arrange
  json::Value arr1;
  arr1.push_back("value1");
  arr1.push_back("value2");
  arr1.push_back("value3");

  json::Value arr2;
  arr2.push_back("valueA");
  arr2.push_back("valueB");

  // act
  arr2[1] = std::move(arr1[0]);

  // assert
  json::Array expected_arr1;
  expected_arr1.get().push_back(new json::String("value2"));
  expected_arr1.get().push_back(new json::String("value3"));

  json::Array expected_arr2;
  expected_arr2.get().push_back(new json::String("valueA"));
  expected_arr2.get().push_back(new json::String("value1"));

  ASSERT_EQ(arr1, expected_arr1);
  ASSERT_EQ(arr2, expected_arr2);
}

TEST_F(ValueTest, ObjectRemove) {
  delete root_;
  delete array_;
  delete nested_obj_;

  // arrange
  json::Value obj;
  obj["key1"] = "value1";
  obj["key2"] = "value2";
  obj["key3"] = "value3";

  // act
  obj.remove("key2");

  // assert
  json::Object expected_obj;
  expected_obj.get().insert("key1", new json::String("value1"));
  expected_obj.get().insert("key3", new json::String("value3"));

  ASSERT_EQ(obj, expected_obj);
}

TEST_F(ValueTest, ArrayRemove) {
  delete root_;
  delete array_;
  delete nested_obj_;

  // arrange
  json::Value arr;
  arr.push_back("value1");
  arr.push_back("value2");
  arr.push_back("value3");

  // act
  arr.remove(1);  // Remove the element at index 1

  // assert
  json::Array expected_arr;
  expected_arr.get().push_back(new json::String("value1"));
  expected_arr.get().push_back(nullptr);  // index 1 is nullified
  expected_arr.get().push_back(new json::String("value3"));

  ASSERT_EQ(arr, expected_arr);
}

TEST_F(ValueTest, ArrayIteratorBasic) {
  delete root_;
  delete array_;
  delete nested_obj_;

  // arrange
  json::Value arr;
  arr.push_back("value1");
  arr.push_back("value2");
  arr.push_back("value3");

  // act
  json::Value::Iterator it = arr.begin();

  // assert
  ASSERT_EQ(*it, "value1");
}

TEST_F(ValueTest, ArrayIteratorIncrement) {
  delete root_;
  delete array_;
  delete nested_obj_;

  // arrange
  json::Value arr;
  arr.push_back("value1");
  arr.push_back("value2");

  // act
  json::Value::Iterator it = arr.begin();
  ++it;

  // assert
  ASSERT_EQ(*it, "value2");
}

TEST_F(ValueTest, ArrayIteratorDecrement) {
  delete root_;
  delete array_;
  delete nested_obj_;

  // arrange
  json::Value arr;
  arr.push_back("value1");
  arr.push_back("value2");

  // act
  json::Value::Iterator it = arr.begin();
  ++it;
  --it;

  // assert
  ASSERT_EQ(*it, "value1");
}

TEST_F(ValueTest, ArrayIteratorEquality) {
  delete root_;
  delete array_;
  delete nested_obj_;

  // arrange
  json::Value arr;
  arr.push_back("value1");
  arr.push_back("value2");

  // act
  json::Value::Iterator it1 = arr.begin();
  json::Value::Iterator it2 = arr.begin();

  // assert
  ASSERT_EQ(it1, it2);
  ++it1;
  ASSERT_NE(it1, it2);
}

TEST_F(ValueTest, ArrayIteratorEndEquality) {
  delete root_;
  delete array_;
  delete nested_obj_;

  // arrange
  json::Value arr;
  arr.push_back("value1");

  // act
  json::Value::Iterator it = arr.begin();
  ++it;

  // assert
  ASSERT_EQ(it, arr.end());
}

// TODO(think about how i want to implement iterators)
TEST_F(ValueTest, DISABLED_ArrayIteratorOutOfRange) {
  delete root_;
  delete array_;
  delete nested_obj_;

  // arrange
  json::Value arr;
  arr.push_back("value1");
  json::Value::Iterator it = arr.begin();
  ++it;  // now at end

  // act & assert
  ASSERT_THROW(++it, std::out_of_range);
}

// TODO(think about how i want to implement iterators)
TEST_F(ValueTest, DISABLED_ArrayIteratorDecrementBegin) {
  delete root_;
  delete array_;
  delete nested_obj_;

  // arrange
  json::Value arr;
  arr.push_back("value1");
  json::Value::Iterator it = arr.begin();

  // act & assert
  ASSERT_THROW(--it, std::out_of_range);
}

TEST_F(ValueTest, ObjectIteratorBasic) {
  delete root_;
  delete array_;
  delete nested_obj_;

  // arrange
  json::Value obj;
  obj.insert("key1", "value1");
  obj.insert("key2", "value2");
  obj.insert("key3", "value3");

  // act
  json::Value::Iterator it = obj.begin();

  // assert
  ASSERT_EQ(*it, "value1");
}

TEST_F(ValueTest, ObjectIteratorIncrement) {
  delete root_;
  delete array_;
  delete nested_obj_;

  // arrange
  json::Value obj;
  obj.insert("key1", "value1");
  obj.insert("key2", "value2");

  // act
  json::Value::Iterator it = obj.begin();
  ++it;

  // assert
  ASSERT_EQ(*it, "value2");
}

TEST_F(ValueTest, ObjectIteratorDecrement) {
  delete root_;
  delete array_;
  delete nested_obj_;

  // arrange
  json::Value obj;
  obj.insert("key1", "value1");
  obj.insert("key2", "value2");

  // act
  json::Value::Iterator it = obj.begin();
  ++it;
  --it;

  // assert
  ASSERT_EQ(*it, "value1");
}

TEST_F(ValueTest, ObjectIteratorEquality) {
  delete root_;
  delete array_;
  delete nested_obj_;

  // arrange
  json::Value obj;
  obj.insert("key1", "value1");
  obj.insert("key2", "value2");

  // act
  json::Value::Iterator it1 = obj.begin();
  json::Value::Iterator it2 = obj.begin();

  // assert
  ASSERT_EQ(it1, it2);
  ++it1;
  ASSERT_NE(it1, it2);
}

TEST_F(ValueTest, ObjectIteratorEndEquality) {
  delete root_;
  delete array_;
  delete nested_obj_;

  // arrange
  json::Value obj;
  obj.insert("key1", "value1");

  // act
  json::Value::Iterator it = obj.begin();
  ++it;

  // assert
  ASSERT_EQ(it, obj.end());
}

// TODO(think about how i want to implement iterators)
TEST_F(ValueTest, DISABLED_ObjectIteratorOutOfRange) {
  delete root_;
  delete array_;
  delete nested_obj_;

  // arrange
  json::Value obj;
  obj.insert("key1", "value1");
  json::Value::Iterator it = obj.begin();
  ++it;  // now at end

  // act & assert
  ASSERT_THROW(++it, std::out_of_range);
}

TEST_F(ValueTest, ObjectIteratorModification) {
  delete root_;
  delete array_;
  delete nested_obj_;

  // arrange
  json::Value obj;
  obj.insert("key1", "value1");
  obj.insert("key2", "value2");

  // act
  json::Value::Iterator it = obj.begin();

  *it = 10;

  // assert
  ASSERT_EQ(obj["key1"], 10);
}

TEST_F(ValueTest, ArrayConstIteratorBasic) {
  delete root_;
  delete array_;
  delete nested_obj_;

  // arrange
  json::Value arr;
  arr.push_back("value1");
  arr.push_back("value2");
  arr.push_back("value3");
  // act
  json::Value::ConstIterator it = arr.cbegin();
  // assert
  ASSERT_EQ(*it, "value1");
}

TEST_F(ValueTest, ArrayConstIteratorIncrement) {
  delete root_;
  delete array_;
  delete nested_obj_;

  // arrange
  json::Value arr;
  arr.push_back("value1");
  arr.push_back("value2");
  // act
  json::Value::ConstIterator it = arr.cbegin();
  ++it;
  // assert
  ASSERT_EQ(*it, "value2");
}

TEST_F(ValueTest, ArrayConstIteratorDecrement) {
  delete root_;
  delete array_;
  delete nested_obj_;

  // arrange
  json::Value arr;
  arr.push_back("value1");
  arr.push_back("value2");
  // act
  json::Value::ConstIterator it = arr.cbegin();
  ++it;
  --it;
  // assert
  ASSERT_EQ(*it, "value1");
}

TEST_F(ValueTest, ArrayConstIteratorEquality) {
  delete root_;
  delete array_;
  delete nested_obj_;

  // arrange
  json::Value arr;
  arr.push_back("value1");
  arr.push_back("value2");
  // act
  json::Value::ConstIterator it1 = arr.cbegin();
  json::Value::ConstIterator it2 = arr.cbegin();
  // assert
  ASSERT_EQ(it1, it2);
  ++it1;
  ASSERT_NE(it1, it2);
}

TEST_F(ValueTest, ArrayConstIteratorEndEquality) {
  delete root_;
  delete array_;
  delete nested_obj_;

  // arrange
  json::Value arr;
  arr.push_back("value1");
  // act
  json::Value::ConstIterator it = arr.cbegin();
  ++it;
  // assert
  ASSERT_EQ(it, arr.cend());
}

// TODO(think about how i want to implement iterators)
TEST_F(ValueTest, DISABLED_ArrayConstIteratorOutOfRange) {
  delete root_;
  delete array_;
  delete nested_obj_;

  // arrange
  json::Value arr;
  arr.push_back("value1");
  json::Value::ConstIterator it = arr.cbegin();
  ++it;  // now at end
  // act & assert
  ASSERT_THROW(++it, std::out_of_range);
}

// TODO(think about how i want to implement iterators)
TEST_F(ValueTest, DISABLED_ArrayConstIteratorDecrementBegin) {
  delete root_;
  delete array_;
  delete nested_obj_;

  // arrange
  json::Value arr;
  arr.push_back("value1");
  json::Value::ConstIterator it = arr.cbegin();
  // act & assert
  ASSERT_THROW(--it, std::out_of_range);
}

TEST_F(ValueTest, ObjectConstIteratorBasic) {
  delete root_;
  delete array_;
  delete nested_obj_;

  // arrange
  json::Value obj;
  obj.insert("key1", "value1");
  obj.insert("key2", "value2");
  obj.insert("key3", "value3");
  // act
  json::Value::ConstIterator it = obj.cbegin();
  // assert
  ASSERT_EQ(*it, "value1");
}

TEST_F(ValueTest, ObjectConstIteratorIncrement) {
  delete root_;
  delete array_;
  delete nested_obj_;

  // arrange
  json::Value obj;
  obj.insert("key1", "value1");
  obj.insert("key2", "value2");
  // act
  json::Value::ConstIterator it = obj.cbegin();
  ++it;
  // assert
  ASSERT_EQ(*it, "value2");
}

TEST_F(ValueTest, ObjectConstIteratorDecrement) {
  delete root_;
  delete array_;
  delete nested_obj_;

  // arrange
  json::Value obj;
  obj.insert("key1", "value1");
  obj.insert("key2", "value2");
  // act
  json::Value::ConstIterator it = obj.cbegin();
  ++it;
  --it;
  // assert
  ASSERT_EQ(*it, "value1");
}

TEST_F(ValueTest, ObjectConstIteratorEquality) {
  delete root_;
  delete array_;
  delete nested_obj_;

  // arrange
  json::Value obj;
  obj.insert("key1", "value1");
  obj.insert("key2", "value2");
  // act
  json::Value::ConstIterator it1 = obj.cbegin();
  json::Value::ConstIterator it2 = obj.cbegin();
  // assert
  ASSERT_EQ(it1, it2);
  ++it1;
  ASSERT_NE(it1, it2);
}

TEST_F(ValueTest, ObjectConstIteratorEndEquality) {
  delete root_;
  delete array_;
  delete nested_obj_;

  // arrange
  json::Value obj;
  obj.insert("key1", "value1");
  // act
  json::Value::ConstIterator it = obj.cbegin();
  ++it;
  // assert
  ASSERT_EQ(it, obj.cend());
}

// TODO(think about how i want to implement iterators)
TEST_F(ValueTest, DISABLED_ObjectConstIteratorOutOfRange) {
  delete root_;
  delete array_;
  delete nested_obj_;

  // arrange
  json::Value obj;
  obj.insert("key1", "value1");
  json::Value::ConstIterator it = obj.cbegin();
  ++it;  // now at end
  // act & assert
  ASSERT_THROW(++it, std::out_of_range);
}

// TODO(think about how i want to implement iterators)
TEST_F(ValueTest, DISABLED_ObjectConstIteratorDecrementBegin) {
  delete root_;
  delete array_;
  delete nested_obj_;

  // arrange
  json::Value obj;
  obj.insert("key1", "value1");
  json::Value::ConstIterator it = obj.cbegin();
  // act & assert
  ASSERT_THROW(--it, std::out_of_range);
}
