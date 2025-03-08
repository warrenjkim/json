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
  json::Object* root = new json::Object();
  json::Node* string = new json::String("value");
  json::Number num(123);
  root->insert("key", string);

  // act
  json::visitors::SetVisitor visitor(&string, num.clone(), "key");
  root->accept(visitor);

  // assert
  json::Object* expected_root = new json::Object();
  expected_root->insert("key", num.clone());
  ASSERT_EQ(json::Value(root), json::Value(expected_root));
}

TEST_F(SetVisitorTest, SetArrayValue) {
  // arrange
  json::Array* root = new json::Array();
  json::Node* string = new json::String("value");
  json::Number num(123);
  root->push_back(string);

  // act
  json::visitors::SetVisitor visitor(&string, num.clone(), "0");
  root->accept(visitor);

  // assert
  json::Array* expected_root = new json::Array();
  expected_root->push_back(num.clone());
  ASSERT_EQ(json::Value(root), json::Value(expected_root));
}

TEST_F(SetVisitorTest, SetBooleanValueThrowsException) {
  // arrange
  json::Boolean* root = new json::Boolean(true);
  json::Node* string = new json::String("value");
  json::Number* num = new json::Number(123);

  // act + assert
  json::visitors::SetVisitor visitor(&string, num, "key");
  ASSERT_THROW(root->accept(visitor), json::UnexpectedParentException);

  delete root;
  delete string;
  delete num;
}

TEST_F(SetVisitorTest, SetNullValueThrowsException) {
  // arrange
  json::Null* root = new json::Null();
  json::Node* string = new json::String("value");
  json::Number* num = new json::Number(123);

  // act + assert
  json::visitors::SetVisitor visitor(&string, num, "key");
  ASSERT_THROW(root->accept(visitor), json::UnexpectedParentException);

  delete root;
  delete string;
  delete num;
}

TEST_F(SetVisitorTest, SetNumberValueThrowsException) {
  // arrange
  json::Number* root = new json::Number(42);
  json::Node* string = new json::String("value");
  json::Number* num = new json::Number(123);

  // act + assert
  json::visitors::SetVisitor visitor(&string, num, "key");
  ASSERT_THROW(root->accept(visitor), json::UnexpectedParentException);

  delete root;
  delete num;
  delete string;
}

TEST_F(SetVisitorTest, SetStringValueThrowsException) {
  // arrange
  json::String* root = new json::String("test");
  json::Node* string = new json::String("value");
  json::Number* num = new json::Number(123);

  // act + assert
  json::visitors::SetVisitor visitor(&string, num, "key");
  ASSERT_THROW(root->accept(visitor), json::UnexpectedParentException);

  delete root;
  delete num;
  delete string;
}
