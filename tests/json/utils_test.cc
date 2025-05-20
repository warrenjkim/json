#include "warren/json/utils.h"

#include <cstddef>

#include "gtest/gtest.h"
#include "warren/json/exception.h"

class JsonUtilsTest : public ::testing::Test {};

TEST_F(JsonUtilsTest, ParseSimplePrimitives) {
  // Test each primitive type
  EXPECT_EQ(json::parse("[null]")[0], nullptr);
  EXPECT_EQ(json::parse("[true]")[0], true);
  EXPECT_EQ(json::parse("[false]")[0], false);
  EXPECT_EQ(json::parse("[42]")[0], 42);
  EXPECT_EQ(json::parse("[3.14]")[0], 3.14);
  EXPECT_EQ(json::parse("[\"hello\"]")[0], "hello");
}

TEST_F(JsonUtilsTest, ParseArray) {
  json::Value arr = json::parse("[1, \"two\", true, null]");

  EXPECT_EQ(arr[0], 1);
  EXPECT_EQ(arr[1], "two");
  EXPECT_EQ(arr[2], true);
  EXPECT_EQ(arr[3], nullptr);
}

TEST_F(JsonUtilsTest, ParseObject) {
  json::Value obj = json::parse(R"(
   {
     "number": 42,
     "string": "value",
     "boolean": true,
     "null": null
   }
 )");

  EXPECT_EQ(obj["number"], 42);
  EXPECT_EQ(obj["string"], "value");
  EXPECT_EQ(obj["boolean"], true);
  EXPECT_EQ(obj["null"], nullptr);
}

TEST_F(JsonUtilsTest, ParseNestedStructures) {
  json::Value nested = json::parse(R"(
   {
     "array": [1, 2, 3],
     "object": {
       "key": "value"
     }
   }
 )");

  EXPECT_EQ(nested["array"][0], 1);
  EXPECT_EQ(nested["array"][1], 2);
  EXPECT_EQ(nested["array"][2], 3);
  EXPECT_EQ(nested["object"]["key"], "value");
}

// TODO(update this. we no longer throw a ParseException)
TEST_F(JsonUtilsTest, DISABLED_InvalidJson) {
  EXPECT_THROW(json::parse("{"), json::ParseException);
  try {
    json::parse("{");
  } catch (json::ParseException e) {
    EXPECT_EQ(std::string(e.what()), "Parsing failed.");
  }
  EXPECT_THROW(json::parse("[1,]"), json::ParseException);
  EXPECT_THROW(json::parse("invalid"), json::ParseException);
}

TEST_F(JsonUtilsTest, TokenizeValidJson) {
  auto tokens = json::tokenize("{\"key\": 42}");
  EXPECT_TRUE(tokens.has_value());
}

TEST_F(JsonUtilsTest, TokenizeInvalidJson) {
  auto tokens = json::tokenize("{invalid}");
  EXPECT_FALSE(tokens.has_value());
}

TEST_F(JsonUtilsTest, ParseFromTokens) {
  auto tokens = json::tokenize("{\"key\": 42}");
  EXPECT_TRUE(tokens.has_value());

  json::Value obj = json::parse(*tokens);
  EXPECT_EQ(obj["key"], 42);
}

TEST_F(JsonUtilsTest, ComplexExample) {
  json::Value complex = json::parse(R"(
   {
     "name": "test",
     "numbers": [1, 2, 3],
     "nested": {
       "a": true,
       "b": [
         {"x": 1},
         {"y": 2}
       ]
     }
   }
 )");

  EXPECT_EQ(complex["name"], "test");
  EXPECT_EQ(complex["numbers"][0], 1);
  EXPECT_EQ(complex["nested"]["a"], true);
  EXPECT_EQ(complex["nested"]["b"][0]["x"], 1);
  EXPECT_EQ(complex["nested"]["b"][1]["y"], 2);
  EXPECT_EQ(json::to_string(complex),
            "{ \"name\": \"test\", \"nested\": { \"a\": true, \"b\": [ { "
            "\"x\": 1 }, { \"y\": 2 } ] }, \"numbers\": [ 1, 2, 3 ] }");
}
