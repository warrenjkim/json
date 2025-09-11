#include "warren/json/internal/parse/parser.h"

#include <map>
#include <string>

#include "gtest/gtest.h"
#include "warren/json/internal/ast/array.h"
#include "warren/json/internal/ast/boolean.h"
#include "warren/json/internal/ast/node.h"
#include "warren/json/internal/ast/null.h"
#include "warren/json/internal/ast/number.h"
#include "warren/json/internal/ast/object.h"
#include "warren/json/internal/ast/string.h"
#include "warren/json/internal/parse/lexer.h"

TEST(ParserTest, EmptyObject) {
  // arrange
  json::syntax::Parser parser(json::syntax::Lexer("{}"));

  // act
  json::ast::Object* node = (json::ast::Object*)parser.parse();
  std::map<std::string, json::ast::Node*> res = node->value;
  std::map<std::string, json::ast::Node*> map;

  // assert
  EXPECT_EQ(res, map);

  delete node;
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

  std::map<std::string, json::ast::Node*> obj;
  obj["int"] = new json::ast::Number(1);
  obj["str"] = new json::ast::String("two");
  obj["float"] = new json::ast::Number(3.4);
  obj["null"] = new json::ast::Null();
  obj["bool"] = new json::ast::Boolean(true);
  obj["obj"] = new json::ast::Object({});
  obj["arr"] = new json::ast::Array({});

  // act
  json::ast::Object* node = (json::ast::Object*)parser.parse();
  std::map<std::string, json::ast::Node*> res = node->value;

  // assert
  EXPECT_EQ(((json::ast::Number*)obj["int"])->value,
            ((json::ast::Number*)res["int"])->value);
  EXPECT_EQ(((json::ast::String*)obj["str"])->value,
            ((json::ast::String*)res["str"])->value);
  EXPECT_EQ(((json::ast::Number*)obj["float"])->value,
            ((json::ast::Number*)res["float"])->value);
  EXPECT_EQ(((json::ast::Null*)obj["null"])->value,
            ((json::ast::Null*)res["null"])->value);
  EXPECT_EQ(((json::ast::Boolean*)obj["bool"])->value,
            ((json::ast::Boolean*)res["bool"])->value);
  EXPECT_EQ(((json::ast::Object*)obj["obj"])->value,
            ((json::ast::Object*)res["obj"])->value);
  EXPECT_EQ(((json::ast::Array*)obj["arr"])->value,
            ((json::ast::Array*)res["arr"])->value);

  for (auto [_, v] : obj) {
    delete v;
  }

  delete node;
}

TEST(ParserTest, EmptyArray) {
  // arrange
  json::syntax::Parser parser(json::syntax::Lexer("[]"));

  // act
  json::ast::Array* node = (json::ast::Array*)parser.parse();
  std::vector<json::ast::Node*> res = node->value;

  // assert
  EXPECT_EQ(res, std::vector<json::ast::Node*>{});

  delete node;
}

TEST(ParserTest, SimpleArray) {
  // arrange
  json::syntax::Parser parser(
      json::syntax::Lexer("[1, \"two\", 3.4, null, true, {}, []]"));
  std::vector<json::ast::Node*> arr;
  arr.reserve(7);
  arr.push_back(new json::ast::Number(1));
  arr.push_back(new json::ast::String("two"));
  arr.push_back(new json::ast::Number(3.4));
  arr.push_back(new json::ast::Null());
  arr.push_back(new json::ast::Boolean(true));
  arr.push_back(new json::ast::Object({}));
  arr.push_back(new json::ast::Array({}));

  // act
  json::ast::Array* node = (json::ast::Array*)parser.parse();
  std::vector<json::ast::Node*> res = node->value;

  // assert
  EXPECT_EQ(((json::ast::Number*)arr[0])->value,
            ((json::ast::Number*)res[0])->value);
  EXPECT_EQ(((json::ast::String*)arr[1])->value,
            ((json::ast::String*)res[1])->value);
  EXPECT_EQ(((json::ast::Number*)arr[2])->value,
            ((json::ast::Number*)res[2])->value);
  EXPECT_EQ(((json::ast::Null*)arr[3])->value,
            ((json::ast::Null*)res[3])->value);
  EXPECT_EQ(((json::ast::Boolean*)arr[4])->value,
            ((json::ast::Boolean*)res[4])->value);
  EXPECT_EQ(((json::ast::Object*)arr[5])->value,
            ((json::ast::Object*)res[5])->value);
  EXPECT_EQ(((json::ast::Array*)arr[6])->value,
            ((json::ast::Array*)res[6])->value);

  for (json::ast::Node* v : arr) {
    delete v;
  }

  delete node;
}
