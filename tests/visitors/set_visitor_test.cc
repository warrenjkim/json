#include "warren/internal/visitors/set_visitor.h"

#include "gtest/gtest.h"
#include "warren/internal/nodes/array.h"
#include "warren/internal/nodes/boolean.h"
#include "warren/internal/nodes/node.h"
#include "warren/internal/nodes/null.h"
#include "warren/internal/nodes/number.h"
#include "warren/internal/nodes/object.h"
#include "warren/internal/nodes/string.h"
#include "warren/json/exception.h"
#include "warren/json/value.h"

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
  EXPECT_EQ(json::Value(root), json::Value(expected_root));
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
  EXPECT_EQ(json::Value(root), json::Value(expected_root));
}

TEST_F(SetVisitorTest, SetBooleanValueThrowsException) {
  // arrange
  json::nodes::Boolean* root = new json::nodes::Boolean(true);
  json::nodes::Node* string = new json::nodes::String("value");
  json::nodes::Number* num = new json::nodes::Number(json::dsa::Numeric(123));

  // act + assert
  json::visitors::SetVisitor visitor(&string, num, "key");
  EXPECT_THROW(root->accept(visitor), json::UnexpectedParentException);

  delete root;
  delete string;
  delete num;
}

TEST_F(SetVisitorTest, SetNullValueThrowsException) {
  // arrange
  json::nodes::Null* root = new json::nodes::Null();
  json::nodes::Node* string = new json::nodes::String("value");
  json::nodes::Number* num = new json::nodes::Number(json::dsa::Numeric(123));

  // act + assert
  json::visitors::SetVisitor visitor(&string, num, "key");
  EXPECT_THROW(root->accept(visitor), json::UnexpectedParentException);

  delete root;
  delete string;
  delete num;
}

TEST_F(SetVisitorTest, SetNumberValueThrowsException) {
  // arrange
  json::nodes::Number* root = new json::nodes::Number(json::dsa::Numeric(42));
  json::nodes::Node* string = new json::nodes::String("value");
  json::nodes::Number* num = new json::nodes::Number(json::dsa::Numeric(123));

  // act + assert
  json::visitors::SetVisitor visitor(&string, num, "key");
  EXPECT_THROW(root->accept(visitor), json::UnexpectedParentException);

  delete root;
  delete num;
  delete string;
}

TEST_F(SetVisitorTest, SetStringValueThrowsException) {
  // arrange
  json::nodes::String* root = new json::nodes::String("test");
  json::nodes::Node* string = new json::nodes::String("value");
  json::nodes::Number* num = new json::nodes::Number(json::dsa::Numeric(123));

  // act + assert
  json::visitors::SetVisitor visitor(&string, num, "key");
  EXPECT_THROW(root->accept(visitor), json::UnexpectedParentException);

  delete root;
  delete num;
  delete string;
}
