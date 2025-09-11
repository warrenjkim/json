#include "warren/json/value.h"

#include <map>
#include <string>
#include <vector>

#include "gtest/gtest.h"

TEST(JsonValueTest, DefaultConstructor) {
  { EXPECT_EQ(json::Value(), nullptr); }
  {
    json::Value v;
    EXPECT_EQ(v, nullptr);
  }
}

TEST(JsonValueTest, NullConstructor) {
  { EXPECT_EQ(json::Value(nullptr), nullptr); }
  {
    json::Value v = nullptr;
    EXPECT_EQ(v, nullptr);
  }
}

TEST(JsonValueTest, BoolConstructor) {
  { EXPECT_EQ(json::Value(true), true); }
  {
    json::Value v = true;
    EXPECT_EQ(v, true);
  }
}

TEST(JsonValueTest, IntConstructor) {
  { EXPECT_EQ(json::Value(42), 42); }
  {
    json::Value v = 42;
    EXPECT_EQ(v, 42);
  }
}

TEST(JsonValueTest, DoubleConstructor) {
  { EXPECT_DOUBLE_EQ(json::Value(1.23), 1.23); }
  {
    json::Value v = 1.23;
    EXPECT_DOUBLE_EQ(v, 1.23);
  }
}

TEST(JsonValueTest, ArrayConstructor) {
  std::vector<json::Value> arr{nullptr,
                               true,
                               42,
                               1.23,
                               std::vector<json::Value>{},
                               std::map<std::string, json::Value>{}};
  {
    json::Value v(arr);
    EXPECT_EQ(v, arr);
  }
  {
    json::Value v = arr;
    EXPECT_EQ(v, arr);
  }
}

TEST(JsonValueTest, ObjectConstructor) {
  std::map<std::string, json::Value> map{
      {"null", nullptr},
      {"bool", true},
      {"int32_t", 42},
      {"double", 1.23},
      {"array", std::vector<json::Value>{}},
      {"map", std::map<std::string, json::Value>{}}};
  {
    json::Value v(map);
    EXPECT_EQ(v, map);
  }
  {
    json::Value v = map;
    EXPECT_EQ(v, map);
  }
}

TEST(JsonValueTest, CopyConstructor) {
  json::Value original = "test";
  json::Value copy(original);

  EXPECT_EQ(copy, "test");
  EXPECT_EQ(original, "test");
}

TEST(JsonValueTest, CopyAssignment) {
  json::Value original = "test";
  json::Value copy;
  copy = original;

  EXPECT_EQ(copy, "test");
  EXPECT_EQ(original, "test");
}

TEST(JsonValueTest, MoveConstructor) {
  json::Value original = "test";
  json::Value moved(std::move(original));

  EXPECT_EQ(moved, "test");
}

TEST(JsonValueTest, MoveAssignment) {
  json::Value original = "test";
  json::Value moved;
  moved = std::move(original);

  EXPECT_EQ(moved, "test");
}

TEST(JsonValueTest, ArrayPushBack) {
  json::Value v;
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);

  EXPECT_EQ(v.size(), 3);
  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[1], 2);
  EXPECT_EQ(v[2], 3);
}

TEST(JsonValueTest, ObjectInsert) {
  json::Value v;
  v["name"] = "name";
  v.insert("age", 42);

  EXPECT_EQ(v.size(), 2);
  EXPECT_EQ(v["name"], "name");
  EXPECT_EQ(v["age"], 42);
}

TEST(JsonValueTest, TypeReassignment) {
  json::Value v = "string";
  EXPECT_EQ(v, "string");

  v = 42;
  EXPECT_EQ(v, 42);

  v = true;
  EXPECT_EQ(v, true);

  v = 3.14;
  EXPECT_DOUBLE_EQ(v, 3.14);

  v = nullptr;
  EXPECT_EQ(v, nullptr);

  v.push_back(1);
  EXPECT_EQ(v, json::array_t{1});

  v = nullptr;
  EXPECT_EQ(v, nullptr);

  v["key"] = json::array_t{1};
  json::object_t map = json::object_t{{"key", json::array_t{1}}};
  EXPECT_EQ(v, map);
}

TEST(JsonValueTest, ArrayErase) {
  json::Value v;
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);

  v.erase(((json::array_t&)v).begin() + 1);

  EXPECT_EQ(v.size(), 2);
  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[1], 3);
}

TEST(JsonValueTest, ObjectErase) {
  json::Value v;
  v.insert("key1", "value1");
  v.insert("key2", "value2");

  EXPECT_EQ(v.size(), 2);
  EXPECT_EQ(v["key1"], "value1");

  v.erase("key1");
  EXPECT_EQ(v.size(), 1);
  EXPECT_EQ(v["key2"], "value2");
}

TEST(JsonValueTest, ArrayIteration) {
  json::Value v;
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);

  int32_t sum = 0;
  for (const json::Value& item : (const json::array_t&)v) {
    sum += (int32_t)item;
  }

  EXPECT_EQ(sum, 6);
}

TEST(JsonValueTest, ObjectIteration) {
  json::Value v;
  v["a"] = 1;
  v["b"] = 2;
  v["c"] = 3;

  int32_t sum = 0;
  for (const auto& [_, value] : (const json::object_t&)v) {
    sum += (int32_t)value;
  }

  EXPECT_EQ(sum, 6);
}

TEST(JsonValueTest, TypeErrorsThrow) {
  json::Value v = "string";
  EXPECT_THROW(v[0], json::BadAccessException);
  EXPECT_THROW(v["key"], json::BadAccessException);
  EXPECT_THROW(v.size(), json::BadAccessException);
  EXPECT_THROW(v.empty(), json::BadAccessException);
  EXPECT_THROW((void)(bool)v, json::BadAccessException);
  EXPECT_THROW(v.push_back(1), json::BadAccessException);
  EXPECT_THROW((void)(double)v, json::BadAccessException);
  EXPECT_THROW((void)(int32_t)v, json::BadAccessException);
  EXPECT_THROW(v.insert("key", 1), json::BadAccessException);
}
