#include "visitors/set_visitor.h"

#include <gtest/gtest.h>

#include "json/value.h"
#include "nodes/array.h"
#include "nodes/boolean.h"
#include "nodes/node.h"
#include "nodes/null.h"
#include "nodes/number.h"
#include "nodes/object.h"
#include "nodes/string.h"

class SetVisitorTest : public ::testing::Test {};

TEST_F(SetVisitorTest, SetObjectValue) {
  // arrange
  json::nodes::Object* root = new json::nodes::Object();
  json::nodes::Node* string = new json::nodes::String("value");
  json::nodes::Number num(123);
  root->insert("key", string);

  // act
  json::visitors::SetVisitor visitor(&string, num.clone(), "key");
  root->accept(visitor);

  // assert
  json::nodes::Object* expected_root = new json::nodes::Object();
  expected_root->insert("key", num.clone());
  ASSERT_EQ(json::Value(root), json::Value(expected_root));
}

TEST_F(SetVisitorTest, SetArrayValue) {
  // arrange
  json::nodes::Array* root = new json::nodes::Array();
  json::nodes::Node* string = new json::nodes::String("value");
  json::nodes::Number num(123);
  root->push_back(string);

  // act
  json::visitors::SetVisitor visitor(&string, num.clone(), "0");
  root->accept(visitor);

  // assert
  json::nodes::Array* expected_root = new json::nodes::Array();
  expected_root->push_back(num.clone());
  ASSERT_EQ(json::Value(root), json::Value(expected_root));
}

TEST_F(SetVisitorTest, SetBooleanValueThrowsException) {
  // arrange
  json::nodes::Boolean* root = new json::nodes::Boolean(true);
  json::nodes::Node* string = new json::nodes::String("value");
  json::nodes::Number* num = new json::nodes::Number(123);

  // act + assert
  json::visitors::SetVisitor visitor(&string, num, "key");
  ASSERT_THROW(root->accept(visitor), json::UnexpectedParentException);

  delete root;
  delete string;
  delete num;
}

TEST_F(SetVisitorTest, SetNullValueThrowsException) {
  // arrange
  json::nodes::Null* root = new json::nodes::Null();
  json::nodes::Node* string = new json::nodes::String("value");
  json::nodes::Number* num = new json::nodes::Number(123);

  // act + assert
  json::visitors::SetVisitor visitor(&string, num, "key");
  ASSERT_THROW(root->accept(visitor), json::UnexpectedParentException);

  delete root;
  delete string;
  delete num;
}

TEST_F(SetVisitorTest, SetNumberValueThrowsException) {
  // arrange
  json::nodes::Number* root = new json::nodes::Number(42);
  json::nodes::Node* string = new json::nodes::String("value");
  json::nodes::Number* num = new json::nodes::Number(123);

  // act + assert
  json::visitors::SetVisitor visitor(&string, num, "key");
  ASSERT_THROW(root->accept(visitor), json::UnexpectedParentException);

  delete root;
  delete num;
  delete string;
}

TEST_F(SetVisitorTest, SetStringValueThrowsException) {
  // arrange
  json::nodes::String* root = new json::nodes::String("test");
  json::nodes::Node* string = new json::nodes::String("value");
  json::nodes::Number* num = new json::nodes::Number(123);

  // act + assert
  json::visitors::SetVisitor visitor(&string, num, "key");
  ASSERT_THROW(root->accept(visitor), json::UnexpectedParentException);

  delete root;
  delete num;
  delete string;
}
