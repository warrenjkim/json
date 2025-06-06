#include "warren/internal/visitors/stringify_visitor.h"

#include "gtest/gtest.h"
#include "warren/internal/nodes/array.h"
#include "warren/internal/nodes/boolean.h"
#include "warren/internal/nodes/node.h"
#include "warren/internal/nodes/null.h"
#include "warren/internal/nodes/number.h"
#include "warren/internal/nodes/object.h"
#include "warren/internal/nodes/string.h"

class StringifyVisitorTest : public ::testing::Test {};

TEST_F(StringifyVisitorTest, Boolean) {
  json::visitors::StringifyVisitor visitor;

  json::nodes::Boolean(true).accept(visitor);
  EXPECT_EQ(visitor.result(), "true");

  visitor = json::visitors::StringifyVisitor();
  json::nodes::Boolean(false).accept(visitor);
  EXPECT_EQ(visitor.result(), "false");
}

TEST_F(StringifyVisitorTest, Null) {
  json::visitors::StringifyVisitor visitor;

  json::nodes::Null().accept(visitor);
  EXPECT_EQ(visitor.result(), "null");
}

TEST_F(StringifyVisitorTest, Number) {
  json::visitors::StringifyVisitor visitor;

  json::nodes::Number(json::dsa::Numeric(10)).accept(visitor);
  EXPECT_EQ(visitor.result(), "10");

  visitor = json::visitors::StringifyVisitor();
  json::nodes::Number(json::dsa::Numeric(12.34)).accept(visitor);
  EXPECT_TRUE(visitor.result().starts_with("12.34"));
}

TEST_F(StringifyVisitorTest, String) {
  json::visitors::StringifyVisitor visitor;

  json::nodes::String("some string").accept(visitor);
  EXPECT_EQ(visitor.result(), "\"some string\"");
}

TEST_F(StringifyVisitorTest, EmptyArray) {
  json::visitors::StringifyVisitor visitor;

  json::nodes::Array().accept(visitor);
  EXPECT_EQ(visitor.result(), "[]");
}

TEST_F(StringifyVisitorTest, OneElementArray) {
  json::visitors::StringifyVisitor visitor;

  json::nodes::Array arr;
  arr.push_back(new json::nodes::Null());
  arr.accept(visitor);
  EXPECT_EQ(visitor.result(), "[ null ]");
}

TEST_F(StringifyVisitorTest, MultipleElementArray) {
  json::visitors::StringifyVisitor visitor;

  json::nodes::Array arr;
  arr.push_back(new json::nodes::Null());
  arr.push_back(new json::nodes::String("10"));
  arr.accept(visitor);
  EXPECT_EQ(visitor.result(), "[ null, \"10\" ]");
}

TEST_F(StringifyVisitorTest, NestedArray) {
  json::visitors::StringifyVisitor visitor;

  json::nodes::Array arr;
  arr.push_back(new json::nodes::Array());
  arr.push_back(new json::nodes::String("10"));
  arr.accept(visitor);
  EXPECT_EQ(visitor.result(), "[ [], \"10\" ]");
}

TEST_F(StringifyVisitorTest, EmptyObject) {
  json::visitors::StringifyVisitor visitor;

  json::nodes::Object().accept(visitor);
  EXPECT_EQ(visitor.result(), "{}");
}

TEST_F(StringifyVisitorTest, OneElementObject) {
  json::visitors::StringifyVisitor visitor;

  json::nodes::Object obj;
  obj.insert("null", new json::nodes::Null());
  obj.accept(visitor);
  EXPECT_EQ(visitor.result(), "{ \"null\": null }");
}

TEST_F(StringifyVisitorTest, MultipleElementObject) {
  json::visitors::StringifyVisitor visitor;

  json::nodes::Object obj;
  obj.insert("null", new json::nodes::Null());
  obj.insert("string", new json::nodes::String("10"));
  obj.accept(visitor);
  EXPECT_EQ(visitor.result(), "{ \"null\": null, \"string\": \"10\" }");
}

TEST_F(StringifyVisitorTest, NestedObject) {
  json::visitors::StringifyVisitor visitor;

  json::nodes::Object obj;
  obj.insert("object", new json::nodes::Object());
  obj.insert("string", new json::nodes::String("10"));
  obj.accept(visitor);
  EXPECT_EQ(visitor.result(), "{ \"object\": {}, \"string\": \"10\" }");
}
