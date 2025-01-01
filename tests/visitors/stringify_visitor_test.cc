#include "visitors/stringify_visitor.h"

#include <gtest/gtest.h>

#include <boost/log/trivial.hpp>

#include "nodes/array.h"
#include "nodes/boolean.h"
#include "nodes/node.h"
#include "nodes/null.h"
#include "nodes/number.h"
#include "nodes/object.h"
#include "nodes/string.h"
#include "utils/logger.h"

class StringifyVisitorTest : public ::testing::Test {
 protected:
  void SetUp() override {
    json::utils::init_logging(boost::log::trivial::debug);
  }
};

TEST_F(StringifyVisitorTest, Boolean) {
  json::visitors::StringifyVisitor visitor;

  json::Boolean(true).accept(visitor);
  ASSERT_EQ(visitor.result(), "true");

  visitor = json::visitors::StringifyVisitor();
  json::Boolean(false).accept(visitor);
  ASSERT_EQ(visitor.result(), "false");
}

TEST_F(StringifyVisitorTest, Null) {
  json::visitors::StringifyVisitor visitor;

  json::Null().accept(visitor);
  ASSERT_EQ(visitor.result(), "null");
}

TEST_F(StringifyVisitorTest, Number) {
  json::visitors::StringifyVisitor visitor;

  json::Number(10, true).accept(visitor);
  ASSERT_EQ(visitor.result(), "10");

  visitor = json::visitors::StringifyVisitor();
  json::Number(12.34, false).accept(visitor);
  ASSERT_EQ(visitor.result(), "12.34");
}

TEST_F(StringifyVisitorTest, String) {
  json::visitors::StringifyVisitor visitor;

  json::String("some string").accept(visitor);
  ASSERT_EQ(visitor.result(), "\"some string\"");
}

TEST_F(StringifyVisitorTest, EmptyArray) {
  json::visitors::StringifyVisitor visitor;

  json::Array().accept(visitor);
  ASSERT_EQ(visitor.result(), "[]");
}

TEST_F(StringifyVisitorTest, OneElementArray) {
  json::visitors::StringifyVisitor visitor;

  json::Array arr;
  arr.add(new json::Null());
  arr.accept(visitor);
  ASSERT_EQ(visitor.result(), "[ null ]");
}

TEST_F(StringifyVisitorTest, MultipleElementArray) {
  json::visitors::StringifyVisitor visitor;

  json::Array arr;
  arr.add(new json::Null());
  arr.add(new json::String("10"));
  arr.accept(visitor);
  ASSERT_EQ(visitor.result(), "[ null, \"10\" ]");
}

TEST_F(StringifyVisitorTest, NestedArray) {
  json::visitors::StringifyVisitor visitor;

  json::Array arr;
  arr.add(new json::Array());
  arr.add(new json::String("10"));
  arr.accept(visitor);
  ASSERT_EQ(visitor.result(), "[ [], \"10\" ]");
}

TEST_F(StringifyVisitorTest, EmptyObject) {
  json::visitors::StringifyVisitor visitor;

  json::Object().accept(visitor);
  ASSERT_EQ(visitor.result(), "{}");
}

TEST_F(StringifyVisitorTest, OneElementObject) {
  json::visitors::StringifyVisitor visitor;

  json::Object obj;
  obj.add("null", new json::Null());
  obj.accept(visitor);
  ASSERT_EQ(visitor.result(), "{ \"null\": null }");
}

TEST_F(StringifyVisitorTest, MultipleElementObject) {
  json::visitors::StringifyVisitor visitor;

  json::Object obj;
  obj.add("null", new json::Null());
  obj.add("string", new json::String("10"));
  obj.accept(visitor);
  ASSERT_EQ(visitor.result(), "{ \"null\": null, \"string\": \"10\" }");
}

TEST_F(StringifyVisitorTest, NestedObject) {
  json::visitors::StringifyVisitor visitor;

  json::Object obj;
  obj.add("object", new json::Object());
  obj.add("string", new json::String("10"));
  obj.accept(visitor);
  ASSERT_EQ(visitor.result(), "{ \"object\": {}, \"string\": \"10\" }");
}
