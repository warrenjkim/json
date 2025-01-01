#include "utils/json_utils.h"

#include <gtest/gtest.h>

#include <boost/log/trivial.hpp>
#include <cstddef>

#include "json/exception.h"
#include "utils/logger.h"

class JsonUtilsTest : public ::testing::Test {
 protected:
  void SetUp() override {
    json::utils::init_logging(boost::log::trivial::debug);
  }

  void TearDown() override {}
};

TEST_F(JsonUtilsTest, ParseSimplePrimitives) {
  // Test each primitive type
  ASSERT_EQ(json::parse("[null]")[0], nullptr);
  ASSERT_EQ(json::parse("[true]")[0], true);
  ASSERT_EQ(json::parse("[false]")[0], false);
  ASSERT_EQ(json::parse("[42]")[0], 42);
  ASSERT_EQ(json::parse("[3.14]")[0], 3.14);
  ASSERT_EQ(json::parse("[\"hello\"]")[0], "hello");
}

TEST_F(JsonUtilsTest, ParseArray) {
  json::Value arr = json::parse("[1, \"two\", true, null]");

  ASSERT_EQ(arr[0], 1);
  ASSERT_EQ(arr[1], "two");
  ASSERT_EQ(arr[2], true);
  ASSERT_EQ(arr[3], nullptr);
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

  ASSERT_EQ(obj["number"], 42);
  ASSERT_EQ(obj["string"], "value");
  ASSERT_EQ(obj["boolean"], true);
  ASSERT_EQ(obj["null"], nullptr);
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

  ASSERT_EQ(nested["array"][0], 1);
  ASSERT_EQ(nested["array"][1], 2);
  ASSERT_EQ(nested["array"][2], 3);
  ASSERT_EQ(nested["object"]["key"], "value");
}

TEST_F(JsonUtilsTest, InvalidJson) {
  ASSERT_THROW(json::parse("{"), json::ParseException);
  try {
    json::parse("{");
  } catch (json::ParseException e) {
    ASSERT_EQ(std::string(e.what()), "Parsing failed.");
  }
  ASSERT_THROW(json::parse("[1,]"), json::ParseException);
  ASSERT_THROW(json::parse("invalid"), json::ParseException);
}

TEST_F(JsonUtilsTest, TokenizeValidJson) {
  auto tokens = json::tokenize("{\"key\": 42}");
  ASSERT_TRUE(tokens.has_value());
}

TEST_F(JsonUtilsTest, TokenizeInvalidJson) {
  auto tokens = json::tokenize("{invalid}");
  ASSERT_FALSE(tokens.has_value());
}

TEST_F(JsonUtilsTest, ParseFromTokens) {
  auto tokens = json::tokenize("{\"key\": 42}");
  ASSERT_TRUE(tokens.has_value());

  json::Value obj = json::parse(*tokens);
  ASSERT_EQ(obj["key"], 42);
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

  ASSERT_EQ(complex["name"], "test");
  ASSERT_EQ(complex["numbers"][0], 1);
  ASSERT_EQ(complex["nested"]["a"], true);
  ASSERT_EQ(complex["nested"]["b"][0]["x"], 1);
  ASSERT_EQ(complex["nested"]["b"][1]["y"], 2);
  ASSERT_EQ(json::to_string(complex),
            "{ \"name\": \"test\", \"nested\": { \"a\": true, \"b\": [ { "
            "\"x\": 1 }, { \"y\": 2 } ] }, \"numbers\": [ 1, 2, 3 ] }");
}
