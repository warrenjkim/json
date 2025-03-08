#include "visitors/cmp_visitor.h"

#include <gtest/gtest.h>

#include "nodes/array.h"
#include "nodes/boolean.h"
#include "nodes/node.h"
#include "nodes/null.h"
#include "nodes/number.h"
#include "nodes/object.h"
#include "nodes/string.h"

class CmpVisitorTest : public ::testing::Test {
 protected:
  void assert_equal(json::Node* lhs, json::Node* rhs) {
    json::visitors::CmpVisitor visitor(rhs);
    lhs->accept(visitor);
    ASSERT_TRUE(visitor.result()) << "ASTs are not equal";
  }

  void assert_not_equal(json::Node* lhs, json::Node* rhs) {
    json::visitors::CmpVisitor visitor(rhs);
    lhs->accept(visitor);
    ASSERT_FALSE(visitor.result()) << "ASTs are unexpectedly equal";
  }
};

TEST_F(CmpVisitorTest, EmptyObject) {
  // arrange
  json::Object lhs;
  json::Object rhs;

  // act + assert
  assert_equal(&lhs, &rhs);
}

TEST_F(CmpVisitorTest, SimpleObject) {
  // arrange
  json::Object lhs;
  lhs.insert("key", new json::String("value"));

  json::Object rhs;
  rhs.insert("key", new json::String("value"));

  // act + assert
  assert_equal(&lhs, &rhs);
}

TEST_F(CmpVisitorTest, DifferentObjectSizes) {
  // arrange
  json::Object lhs;
  lhs.insert("key_1", new json::String("value_1"));

  json::Object rhs;

  // act + assert
  assert_not_equal(&lhs, &rhs);
}

TEST_F(CmpVisitorTest, ObjectKeyOrder) {
  json::Object lhs;
  lhs.insert("a", new json::Number(1));
  lhs.insert("b", new json::Number(2));

  json::Object rhs;
  rhs.insert("b", new json::Number(2));
  rhs.insert("a", new json::Number(1));

  assert_equal(&lhs, &rhs);
}

TEST_F(CmpVisitorTest, ComplexStructure) {
  json::Object lhs;
  lhs.insert("nested", new json::Object());
  lhs.insert("list", new json::Array());

  json::Object rhs;
  rhs.insert("nested", new json::Object());
  rhs.insert("list", new json::Array());

  assert_equal(&lhs, &rhs);
}

TEST_F(CmpVisitorTest, DifferentComplexStructures) {
  json::Object lhs;
  lhs.insert("name", new json::String("john"));
  lhs.insert("age", new json::Number(30));

  json::Object rhs;
  rhs.insert("name", new json::String("john"));
  rhs.insert("age", new json::Number(31));

  assert_not_equal(&lhs, &rhs);
}

TEST_F(CmpVisitorTest, EmptyArray) {
  // arrange
  json::Array lhs;
  json::Array rhs;

  // act + assert
  assert_equal(&lhs, &rhs);
}

TEST_F(CmpVisitorTest, SimpleArray) {
  // arrange
  json::Array lhs;
  lhs.push_back(new json::Number(1));
  lhs.push_back(new json::Number(2));

  json::Array rhs;
  rhs.push_back(new json::Number(1));
  rhs.push_back(new json::Number(2));

  // act + assert
  assert_equal(&lhs, &rhs);
}

TEST_F(CmpVisitorTest, DifferentNumbers) {
  // arrange
  json::Array lhs;
  lhs.push_back(new json::Number(1));

  json::Array rhs;
  rhs.push_back(new json::Number(2));

  // act + assert
  assert_not_equal(&lhs, &rhs);
}

TEST_F(CmpVisitorTest, NullComparisons) {
  // arrange
  json::Null lhs;
  json::Number rhs(1);

  // act + assert
  assert_equal(&lhs, &lhs);
  assert_not_equal(&lhs, &rhs);
}

TEST_F(CmpVisitorTest, ArrayOrderMatters) {
  // arrange
  json::Array lhs;
  lhs.push_back(new json::Number(1));
  lhs.push_back(new json::Boolean(true));

  json::Array rhs;
  rhs.push_back(new json::Boolean(true));
  rhs.push_back(new json::Number(1));

  // act + assert
  assert_not_equal(&lhs, &rhs);
}

TEST_F(CmpVisitorTest, NumberPrecision) {
  // arrange
  json::Number lhs(1.0);
  json::Number rhs(1);

  // act + assert
  assert_equal(&lhs, &rhs);

  // arrange
  lhs = json::Number(0.1);
  rhs = json::Number(0.1000001);

  // act + assert
  assert_not_equal(&lhs, &rhs);
}

TEST_F(CmpVisitorTest, Strings) {
  // arrange
  json::Array lhs;
  lhs.push_back(new json::String("hello"));
  lhs.push_back(new json::String("world"));

  json::Array rhs;
  rhs.push_back(new json::String("hello"));
  rhs.push_back(new json::String("world"));

  // act + assert
  assert_equal(&lhs, &rhs);
}

TEST_F(CmpVisitorTest, MixedTypes) {
  // arrange
  json::Array lhs;
  lhs.push_back(new json::Number(123));
  lhs.push_back(new json::String("123"));

  json::Array rhs;
  rhs.push_back(new json::String("123"));
  rhs.push_back(new json::Number(123));

  // act + assert
  assert_not_equal(&lhs, &rhs);
}
