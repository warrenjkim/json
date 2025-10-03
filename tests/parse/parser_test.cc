#include "warren/json/internal/parse/parser.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "warren/json/internal/parse/lexer.h"
#include "warren/json/utils/types.h"

namespace json {
namespace syntax {

namespace {

using ::testing::Eq;

TEST(ParserTest, EmptyObject) {
  EXPECT_THAT(Parser(Lexer("{}")).parse(), Eq(object_t{}));
}

TEST(ParserTest, SimpleObject) {
  EXPECT_THAT(Parser(Lexer(R"({
    "int": 1,
    "str": "two",
    "float": 3.4,
    "null": null,
    "bool": true,
    "obj": {},
    "arr": []
  })"))
                  .parse(),
              Eq(object_t{
                  {"int", 1},
                  {"str", "two"},
                  {"float", 3.4},
                  {"null", nullptr},
                  {"bool", true},
                  {"obj", object_t{}},
                  {"arr", array_t{}},
              }));
}

TEST(ParserTest, EmptyArray) {
  EXPECT_THAT(Parser(Lexer("[]")).parse(), Eq(array_t{}));
}

TEST(ParserTest, SimpleArray) {
  EXPECT_THAT(Parser(Lexer("[1, \"two\", 3.4, null, true, {}, []]")).parse(),
              Eq(array_t{1, "two", 3.4, nullptr, true, object_t{}, array_t{}}));
}

}  // namespace

}  // namespace syntax
}  // namespace json
