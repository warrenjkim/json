#include "warren/internal/parse/parser.h"

#include "gtest/gtest.h"
#include "warren/internal/nodes/array.h"
#include "warren/internal/nodes/boolean.h"
#include "warren/internal/nodes/node.h"
#include "warren/internal/nodes/null.h"
#include "warren/internal/nodes/number.h"
#include "warren/internal/nodes/object.h"
#include "warren/internal/nodes/string.h"
#include "warren/internal/parse/lexer.h"

TEST(ParserTest, EmptyObject) {
  // arrange
  json::syntax::Parser parser(json::syntax::Lexer("{}"));
  json::nodes::Object obj;

  // act + assert
  json::nodes::Node* res = parser.parse();
  EXPECT_EQ(*res, obj);

  delete res;
}

TEST(ParserTest, SimpleObject) {
  // arrange
  json::syntax::Parser parser(json::syntax::Lexer(R"({
    "int": 1,
    "str": "two",
    "float": 3.4,
    "null": null,
    "bool": true,
    "obj": {},
    "arr": []
  })"));

  json::nodes::Object obj;
  obj.insert("int", new json::nodes::Number(json::dsa::Numeric(1)));
  obj.insert("str", new json::nodes::String("two"));
  obj.insert("float", new json::nodes::Number(json::dsa::Numeric(3.4)));
  obj.insert("null", new json::nodes::Null());
  obj.insert("bool", new json::nodes::Boolean(true));
  obj.insert("obj", new json::nodes::Object());
  obj.insert("arr", new json::nodes::Array());

  // act + assert
  json::nodes::Node* res = parser.parse();
  EXPECT_EQ(*res, obj);

  delete res;
}

TEST(ParserTest, EmptyArray) {
  // arrange
  json::syntax::Parser parser(json::syntax::Lexer("[]"));
  json::nodes::Array arr;

  // act + assert
  json::nodes::Node* res = parser.parse();
  EXPECT_EQ(*res, arr);

  delete res;
}

TEST(ParserTest, SimpleArray) {
  // arrange
  json::syntax::Parser parser(
      json::syntax::Lexer("[1, \"two\", 3.4, null, true, {}, []]"));
  json::nodes::Array arr;
  arr.push_back(new json::nodes::Number(json::dsa::Numeric(1)));
  arr.push_back(new json::nodes::String("two"));
  arr.push_back(new json::nodes::Number(json::dsa::Numeric(3.4)));
  arr.push_back(new json::nodes::Null());
  arr.push_back(new json::nodes::Boolean(true));
  arr.push_back(new json::nodes::Object());
  arr.push_back(new json::nodes::Array());

  // act + assert
  json::nodes::Node* res = parser.parse();
  EXPECT_EQ(*res, arr);

  delete res;
}
