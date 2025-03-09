#include "json/value.h"

#include <gtest/gtest.h>

#include <cstddef>
#include <string>

#include "nodes/array.h"
#include "nodes/boolean.h"
#include "nodes/node.h"
#include "nodes/null.h"
#include "nodes/number.h"
#include "nodes/object.h"
#include "nodes/string.h"
#include "utils/exception.h"

class ValueTest : public ::testing::Test {
 protected:
  void SetUp() override {
    root_ = new json::nodes::Object();
    root_->insert("null", new json::nodes::Null());
    root_->insert("bool", new json::nodes::Boolean(true));
    root_->insert("number", new json::nodes::Number(42.5));
    root_->insert("string", new json::nodes::String("hello"));

    array_ = new json::nodes::Array();
    array_->push_back(new json::nodes::Number(1));
    array_->push_back(new json::nodes::String("two"));
    array_->push_back(new json::nodes::Boolean(false));
    array_->push_back(new json::nodes::Null());
    root_->insert("array", array_->clone());

    nested_obj_ = new json::nodes::Object();
    nested_obj_->insert("key", new json::nodes::String("value"));
    root_->insert("object", nested_obj_->clone());
  }

  void TearDown() override {}

  json::nodes::Object* root_;
  json::nodes::Array* array_;
  json::nodes::Object* nested_obj_;
};

TEST_F(ValueTest, ConvertNumberToInt) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value(new json::nodes::Number(42.5));
  int result = value;
  ASSERT_EQ(result, 42);
}

TEST_F(ValueTest, ConvertNumberToDouble) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value(new json::nodes::Number(42.5));
  double result = value;
  ASSERT_DOUBLE_EQ(result, 42.5);
}

TEST_F(ValueTest, ConvertNumberToFloat) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value(new json::nodes::Number(42.5));
  float result = value;
  ASSERT_FLOAT_EQ(result, 42.5f);
}

TEST_F(ValueTest, ConvertStringToStdString) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value(new json::nodes::String("test"));
  std::string result = value;
  ASSERT_EQ(result, "test");
}

TEST_F(ValueTest, ConvertStringToCString) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value(new json::nodes::String("test"));
  const char* result = value;
  ASSERT_STREQ(result, "test");
}

TEST_F(ValueTest, ConvertBooleanToBool) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value(new json::nodes::Boolean(true));
  bool result = value;
  ASSERT_TRUE(result);
}

TEST_F(ValueTest, ConvertBooleanToNullPtrT) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value(new json::nodes::Null());
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

  json::Value value(new json::nodes::Number(42));
  ASSERT_TRUE(value == 42);
  ASSERT_TRUE(42 == value);
}

TEST_F(ValueTest, CompareNumberToDouble) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value(new json::nodes::Number(42));
  ASSERT_TRUE(value == 42.0);
  ASSERT_TRUE(42.0 == value);
}

TEST_F(ValueTest, CompareStringToCString) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value(new json::nodes::String("test"));
  ASSERT_TRUE(value == "test");
  ASSERT_TRUE("test" == value);
}

TEST_F(ValueTest, CompareStringToStdString) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value(new json::nodes::String("test"));
  ASSERT_TRUE(value == std::string("test"));
  ASSERT_TRUE(std::string("test") == value);
}

TEST_F(ValueTest, CompareBooleanToBool) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value(new json::nodes::Boolean(true));
  ASSERT_TRUE(value == true);
  ASSERT_TRUE(true == value);
}

TEST_F(ValueTest, CompareValueToValueNumber) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value1(new json::nodes::Number(42));
  json::Value value2(new json::nodes::Number(42));
  ASSERT_TRUE(value1 == value2);
}

TEST_F(ValueTest, CompareValueToValueBool) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value1(new json::nodes::Boolean(true));
  json::Value value2(new json::nodes::Boolean(true));
  ASSERT_TRUE(value1 == value2);
}

TEST_F(ValueTest, CompareValueToValueArray) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value1(new json::nodes::Array());
  json::Value value2(new json::nodes::Array());
  ASSERT_TRUE(value1 == value2);
}

TEST_F(ValueTest, CompareValueToValueObject) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value1(new json::nodes::Object());
  json::Value value2(new json::nodes::Object());
  ASSERT_TRUE(value1 == value2);
}

TEST_F(ValueTest, CompareValueToValueString) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value1(new json::nodes::String("string"));
  json::Value value2(new json::nodes::String("string"));
  ASSERT_TRUE(value1 == value2);
}

TEST_F(ValueTest, CompareValueToValueNotSameType) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value1(new json::nodes::Number(42));
  json::Value value2(new json::nodes::String("string"));
  ASSERT_FALSE(value1 == value2);
}

TEST_F(ValueTest, CompareNullToNullptr) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value(new json::nodes::Null());
  ASSERT_TRUE(value == nullptr);
  ASSERT_TRUE(nullptr == value);
}

TEST_F(ValueTest, BadCastNumberToBool) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value(new json::nodes::Number(42));
  ASSERT_THROW(bool b = value, json::BadCastException);
}

TEST_F(ValueTest, BadCastBooleanToNumber) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value(new json::nodes::Boolean(true));
  ASSERT_THROW(int n = value, json::BadCastException);
}

TEST_F(ValueTest, BadCastBooleanToString) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value(new json::nodes::Boolean(true));
  ASSERT_THROW(std::string s = value, json::BadCastException);
  ASSERT_THROW(const char* cs = value, json::BadCastException);
}

TEST_F(ValueTest, BadCastBooleanToNullptr) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value(new json::nodes::Boolean(true));
  ASSERT_THROW(nullptr_t n = value, json::BadCastException);
}

TEST_F(ValueTest, BadCastNumberToString) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value(new json::nodes::Number(42));
  ASSERT_THROW(std::string s = value, json::BadCastException);
}

TEST_F(ValueTest, BadAccessNumberAsObject) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value(new json::nodes::Number(42));
  ASSERT_THROW((void)value["key"], json::BadAccessException);
}

TEST_F(ValueTest, BadAccessNumberAsArray) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value(new json::nodes::Number(42));
  ASSERT_THROW(value[0], json::BadAccessException);
}

TEST_F(ValueTest, BadCastStringToInt) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value(new json::nodes::String("test"));
  ASSERT_THROW(int i = value, json::BadCastException);
}

TEST_F(ValueTest, BadCastStringToBool) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value(new json::nodes::String("test"));
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

  json::Value value(new json::nodes::Null());
  json::Value other(new json::nodes::Null());
  ASSERT_TRUE(value == other);
}

TEST_F(ValueTest, BadCastNullToInt) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value(new json::nodes::Null());
  ASSERT_THROW(int i = value, json::BadCastException);
}

TEST_F(ValueTest, BadCastNullToBool) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value(new json::nodes::Null());
  ASSERT_THROW(bool b = value, json::BadCastException);
}

TEST_F(ValueTest, BadCastNullToString) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value value(new json::nodes::Null());
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

  json::Value number_val(new json::nodes::Number(42));
  json::Value string_val(new json::nodes::String("test"));
  json::Value bool_val(new json::nodes::Boolean(false));

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

  json::nodes::Array array;
  array.push_back(new json::nodes::Number(10));
  array.push_back(new json::nodes::Boolean(true));

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

  json::nodes::Array array;
  array.push_back(new json::nodes::Boolean(true));
  array.push_back(new json::nodes::Number(10));

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

  json::nodes::Array array;
  array.push_back(new json::nodes::Null());
  array.push_back(new json::nodes::Boolean(true));

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

  json::nodes::Array array;
  array.push_back(new json::nodes::String("string"));
  array.push_back(new json::nodes::Null());

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

  json::nodes::Array array;
  array.push_back(new json::nodes::String("string"));
  array.push_back(new json::nodes::Null());

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

  json::Value num(new json::nodes::Number(8));

  json::nodes::Array array;
  array.push_back(new json::nodes::Number(8));
  array.push_back(new json::nodes::String("string"));

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

  json::nodes::Array array;
  array.push_back(new json::nodes::Null());
  array.push_back(new json::nodes::String("string"));

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

  json::nodes::Object object;
  object.insert("num", new json::nodes::Number(10));
  object.insert("bool", new json::nodes::Boolean(true));

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

  json::nodes::Object object;
  object.insert("bool", new json::nodes::Boolean(true));
  object.insert("num", new json::nodes::Number(10));

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

  json::nodes::Object object;
  object.insert("null", new json::nodes::Null());
  object.insert("bool", new json::nodes::Boolean(true));

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

  json::nodes::Object object;
  object.insert("str", new json::nodes::String("string"));
  object.insert("null", new json::nodes::Null());

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

  json::nodes::Object object;
  object.insert("str", new json::nodes::String("string"));
  object.insert("null", new json::nodes::Null());

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

  json::Value num(new json::nodes::Number(8));

  json::nodes::Object object;
  object.insert("val", new json::nodes::Number(8));
  object.insert("str", new json::nodes::String("string"));

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

  json::nodes::Object object;
  object.insert("empty_val", new json::nodes::Null());
  object.insert("str", new json::nodes::String("string"));

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

  json::Value val(new json::nodes::Number(42));
  val = nullptr;
  ASSERT_EQ(val, nullptr);
}

TEST_F(ValueTest, CopyConstructor) {
  delete root_;
  delete array_;
  delete nested_obj_;

  json::Value val(new json::nodes::Number(42));
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
  json::nodes::Object expected_obj1;
  expected_obj1.get().insert("key2", new json::nodes::String("value2"));

  json::nodes::Object expected_obj2;
  expected_obj2.get().insert("keyA", new json::nodes::String("valueA"));
  expected_obj2.get().insert("keyX", new json::nodes::String("value1"));

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
  json::nodes::Array expected_arr1;
  expected_arr1.get().push_back(new json::nodes::String("value2"));
  expected_arr1.get().push_back(new json::nodes::String("value3"));

  json::nodes::Array expected_arr2;
  expected_arr2.get().push_back(new json::nodes::String("valueA"));
  expected_arr2.get().push_back(new json::nodes::String("value1"));

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
  obj.erase("key2");

  // assert
  json::nodes::Object expected_obj;
  expected_obj.get().insert("key1", new json::nodes::String("value1"));
  expected_obj.get().insert("key3", new json::nodes::String("value3"));

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
  arr.erase(1);  // Remove the element at index 1

  // assert
  json::nodes::Array expected_arr;
  expected_arr.get().push_back(new json::nodes::String("value1"));
  expected_arr.get().push_back(new json::nodes::String("value3"));

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

TEST_F(ValueTest, ArrayErase) {
  delete root_;
  delete nested_obj_;

  // arrange
  json::nodes::Array* expected = new json::nodes::Array();
  expected->push_back(new json::nodes::String("two"));
  expected->push_back(new json::nodes::Boolean(false));
  expected->push_back(new json::nodes::Null());

  // act
  json::Value arr(array_);
  arr.erase(arr.cbegin());

  // assert
  ASSERT_EQ(arr, json::Value(expected));
}

TEST_F(ValueTest, ObjectErase) {
  // arrange
  json::nodes::Object* expected = new json::nodes::Object();
  expected->insert("null", new json::nodes::Null());
  expected->insert("bool", new json::nodes::Boolean(true));
  expected->insert("number", new json::nodes::Number(42.5));
  expected->insert("string", new json::nodes::String("hello"));
  expected->insert("object", nested_obj_->clone());

  // act
  json::Value obj(root_);
  obj.erase(obj.cbegin());

  // assert
  ASSERT_EQ(obj, json::Value(expected));

  delete array_;
  delete nested_obj_;
}

TEST_F(ValueTest, ArrayEraseRange) {
  delete root_;
  delete nested_obj_;

  // arrange
  json::nodes::Array* expected = new json::nodes::Array();
  expected->push_back(new json::nodes::Boolean(false));
  expected->push_back(new json::nodes::Null());

  // act
  json::Value arr(array_);
  json::Value::ConstIterator end = ++(++arr.cbegin());
  arr.erase(arr.cbegin(), end);

  // assert
  ASSERT_EQ(arr, json::Value(expected));
}

TEST_F(ValueTest, ObjectEraseRange) {
  // arrange
  json::nodes::Object* expected = new json::nodes::Object();
  expected->insert("null", new json::nodes::Null());
  expected->insert("number", new json::nodes::Number(42.5));
  expected->insert("string", new json::nodes::String("hello"));
  expected->insert("object", nested_obj_->clone());

  // act
  json::Value obj(root_);
  json::Value::ConstIterator end = ++(++obj.cbegin());
  obj.erase(obj.cbegin(), end);

  // assert
  ASSERT_EQ(obj, json::Value(expected));

  delete array_;
  delete nested_obj_;
}

TEST_F(ValueTest, ArraySize) {
  delete root_;
  delete array_;
  delete nested_obj_;

  // arrange
  json::Value arr;
  ASSERT_TRUE(arr.empty());

  arr.push_back(1);
  arr.push_back("two");

  // act + assert
  ASSERT_EQ(arr.size(), 2);
}

TEST_F(ValueTest, ObjectSize) {
  delete root_;
  delete array_;
  delete nested_obj_;

  // arrange
  json::Value obj;
  ASSERT_TRUE(obj.empty());

  obj["one"] = 1;
  obj["two"] = "two";

  // act + assert
  ASSERT_EQ(obj.size(), 2);
}
