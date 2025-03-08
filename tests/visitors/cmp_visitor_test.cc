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
  void assert_equal(json::nodes::Node* lhs, json::nodes::Node* rhs) {
    json::visitors::CmpVisitor visitor(rhs);
    lhs->accept(visitor);
    ASSERT_TRUE(visitor.result()) << "ASTs are not equal";
  }

  void assert_not_equal(json::nodes::Node* lhs, json::nodes::Node* rhs) {
    json::visitors::CmpVisitor visitor(rhs);
    lhs->accept(visitor);
    ASSERT_FALSE(visitor.result()) << "ASTs are unexpectedly equal";
  }
};

TEST_F(CmpVisitorTest, EmptyObject) {
  // arrange
  json::nodes::Object lhs;
  json::nodes::Object rhs;

  // act + assert
  assert_equal(&lhs, &rhs);
}

TEST_F(CmpVisitorTest, SimpleObject) {
  // arrange
  json::nodes::Object lhs;
  lhs.insert("key", new json::nodes::String("value"));

  json::nodes::Object rhs;
  rhs.insert("key", new json::nodes::String("value"));

  // act + assert
  assert_equal(&lhs, &rhs);
}

TEST_F(CmpVisitorTest, DifferentObjectSizes) {
  // arrange
  json::nodes::Object lhs;
  lhs.insert("key_1", new json::nodes::String("value_1"));

  json::nodes::Object rhs;

  // act + assert
  assert_not_equal(&lhs, &rhs);
}

TEST_F(CmpVisitorTest, ObjectKeyOrder) {
  json::nodes::Object lhs;
  lhs.insert("a", new json::nodes::Number(1));
  lhs.insert("b", new json::nodes::Number(2));

  json::nodes::Object rhs;
  rhs.insert("b", new json::nodes::Number(2));
  rhs.insert("a", new json::nodes::Number(1));

  assert_equal(&lhs, &rhs);
}

TEST_F(CmpVisitorTest, ComplexStructure) {
  json::nodes::Object lhs;
  lhs.insert("nested", new json::nodes::Object());
  lhs.insert("list", new json::nodes::Array());

  json::nodes::Object rhs;
  rhs.insert("nested", new json::nodes::Object());
  rhs.insert("list", new json::nodes::Array());

  assert_equal(&lhs, &rhs);
}

TEST_F(CmpVisitorTest, DifferentComplexStructures) {
  json::nodes::Object lhs;
  lhs.insert("name", new json::nodes::String("john"));
  lhs.insert("age", new json::nodes::Number(30));

  json::nodes::Object rhs;
  rhs.insert("name", new json::nodes::String("john"));
  rhs.insert("age", new json::nodes::Number(31));

  assert_not_equal(&lhs, &rhs);
}

TEST_F(CmpVisitorTest, EmptyArray) {
  // arrange
  json::nodes::Array lhs;
  json::nodes::Array rhs;

  // act + assert
  assert_equal(&lhs, &rhs);
}

TEST_F(CmpVisitorTest, SimpleArray) {
  // arrange
  json::nodes::Array lhs;
  lhs.push_back(new json::nodes::Number(1));
  lhs.push_back(new json::nodes::Number(2));

  json::nodes::Array rhs;
  rhs.push_back(new json::nodes::Number(1));
  rhs.push_back(new json::nodes::Number(2));

  // act + assert
  assert_equal(&lhs, &rhs);
}

TEST_F(CmpVisitorTest, DifferentNumbers) {
  // arrange
  json::nodes::Array lhs;
  lhs.push_back(new json::nodes::Number(1));

  json::nodes::Array rhs;
  rhs.push_back(new json::nodes::Number(2));

  // act + assert
  assert_not_equal(&lhs, &rhs);
}

TEST_F(CmpVisitorTest, NullComparisons) {
  // arrange
  json::nodes::Null lhs;
  json::nodes::Number rhs(1);

  // act + assert
  assert_equal(&lhs, &lhs);
  assert_not_equal(&lhs, &rhs);
}

TEST_F(CmpVisitorTest, ArrayOrderMatters) {
  // arrange
  json::nodes::Array lhs;
  lhs.push_back(new json::nodes::Number(1));
  lhs.push_back(new json::nodes::Boolean(true));

  json::nodes::Array rhs;
  rhs.push_back(new json::nodes::Boolean(true));
  rhs.push_back(new json::nodes::Number(1));

  // act + assert
  assert_not_equal(&lhs, &rhs);
}

TEST_F(CmpVisitorTest, NumberPrecision) {
  // arrange
  json::nodes::Number lhs(1.0);
  json::nodes::Number rhs(1);

  // act + assert
  assert_equal(&lhs, &rhs);

  // arrange
  lhs = json::nodes::Number(0.1);
  rhs = json::nodes::Number(0.1000001);

  // act + assert
  assert_not_equal(&lhs, &rhs);
}

TEST_F(CmpVisitorTest, Strings) {
  // arrange
  json::nodes::Array lhs;
  lhs.push_back(new json::nodes::String("hello"));
  lhs.push_back(new json::nodes::String("world"));

  json::nodes::Array rhs;
  rhs.push_back(new json::nodes::String("hello"));
  rhs.push_back(new json::nodes::String("world"));

  // act + assert
  assert_equal(&lhs, &rhs);
}

TEST_F(CmpVisitorTest, MixedTypes) {
  // arrange
  json::nodes::Array lhs;
  lhs.push_back(new json::nodes::Number(123));
  lhs.push_back(new json::nodes::String("123"));

  json::nodes::Array rhs;
  rhs.push_back(new json::nodes::String("123"));
  rhs.push_back(new json::nodes::Number(123));

  // act + assert
  assert_not_equal(&lhs, &rhs);
}
