#include "warren/json/internal/parse/parser.h"

#include "gtest/gtest.h"
#include "warren/json/internal/parse/lexer.h"
#include "warren/json/utils/types.h"
#include "warren/json/value.h"

TEST(ParserTest, EmptyObject) {
  EXPECT_EQ(json::syntax::Parser(json::syntax::Lexer("{}")).parse(),
            json::object_t{});
}

TEST(ParserTest, SimpleObject) {
  // arrange
  json::object_t obj{
      {"int", 1},
      {"str", "two"},
      {"float", 3.4},
      {"null", nullptr},
      {"bool", true},
      {"obj", json::object_t{}},
      {"arr", json::array_t{}},
  };

  // act
  json::Value res = json::syntax::Parser(json::syntax::Lexer(R"({
    "int": 1,
    "str": "two",
    "float": 3.4,
    "null": null,
    "bool": true,
    "obj": {},
    "arr": []
  })"))
                        .parse();

  // assert
  EXPECT_EQ(obj, res);
}

TEST(ParserTest, EmptyArray) {
  EXPECT_EQ(json::syntax::Parser(json::syntax::Lexer("[]")).parse(),
            json::array_t{});
}

TEST(ParserTest, SimpleArray) {
  // arrange
  json::array_t arr{
      1, "two", 3.4, nullptr, true, json::object_t{}, json::array_t{}};

  // act
  json::Value res =
      json::syntax::Parser(
          json::syntax::Lexer("[1, \"two\", 3.4, null, true, {}, []]"))
          .parse();

  // assert
  EXPECT_EQ(arr, res);
}
