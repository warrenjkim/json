#include "warren/json/internal/parse/lexer.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "warren/json/internal/parse/token.h"

namespace warren {
namespace json {
namespace syntax {

namespace {

using ::testing::Eq;

TEST(LexerTest, LexInvalidNull) {
  Lexer lexer("nul");
  ++lexer;
  EXPECT_FALSE(lexer);
  EXPECT_FALSE(lexer.ok());
  EXPECT_THAT(
      lexer.error(),
      Eq(Lexer::Error(TokenType::JSON_NULL,
                      /*pos=*/0,
                      "incomplete literal: got 'nul', expected 'null'")));
  EXPECT_THAT(*lexer, Eq(Token("nul", TokenType::UNKNOWN)));
}

TEST(LexerTest, LexInvalidTrue) {
  Lexer lexer("tru");
  ++lexer;
  EXPECT_FALSE(lexer);
  EXPECT_FALSE(lexer.ok());
  EXPECT_THAT(
      lexer.error(),
      Eq(Lexer::Error(TokenType::BOOLEAN,
                      /*pos=*/0,
                      "incomplete literal: got 'tru', expected 'true'")));
  EXPECT_THAT(*lexer, Eq(Token("tru", TokenType::UNKNOWN)));
}

TEST(LexerTest, LexInvalidFalse) {
  Lexer lexer("fals");
  ++lexer;
  EXPECT_FALSE(lexer);
  EXPECT_FALSE(lexer.ok());
  EXPECT_THAT(
      lexer.error(),
      Eq(Lexer::Error(TokenType::BOOLEAN,
                      /*pos=*/0,
                      "incomplete literal: got 'fals', expected 'false'")));
  EXPECT_THAT(*lexer, Eq(Token("fals", TokenType::UNKNOWN)));
}

TEST(LexerTest, LexNull) {
  Lexer lexer("null");
  ++lexer;
  EXPECT_TRUE(lexer);
  EXPECT_TRUE(lexer.ok());
  EXPECT_THAT(*lexer, Eq(Token("null", TokenType::JSON_NULL)));
}

TEST(LexerTest, LexTrue) {
  Lexer lexer("true");
  ++lexer;
  EXPECT_TRUE(lexer);
  EXPECT_TRUE(lexer.ok());
  EXPECT_THAT(*lexer, Eq(Token("true", TokenType::BOOLEAN)));
}

TEST(LexerTest, LexFalse) {
  Lexer lexer("false");
  ++lexer;
  EXPECT_TRUE(lexer);
  EXPECT_TRUE(lexer.ok());
  EXPECT_THAT(*lexer, Eq(Token("false", TokenType::BOOLEAN)));
}

TEST(LexerTest, LexInvalidUnterminatedString) {
  Lexer lexer("\"hello");
  ++lexer;
  EXPECT_FALSE(lexer);
  EXPECT_FALSE(lexer.ok());
  EXPECT_THAT(lexer.error(),
              Eq(Lexer::Error(TokenType::QUOTE,
                              /*pos=*/0, "unterminated string")));
  EXPECT_THAT(*lexer, Eq(Token("hello", TokenType::UNKNOWN)));
}

TEST(LexerTest, LexInvalidBadEscapeChar) {
  Lexer lexer("\"hello\\z\"");
  ++lexer;
  EXPECT_FALSE(lexer);
  EXPECT_FALSE(lexer.ok());
  EXPECT_THAT(
      lexer.error(),
      Eq(Lexer::Error(TokenType::STRING,
                      /*pos=*/6, "invalid control character: hello\\z")));
  EXPECT_THAT(*lexer, Eq(Token("hello\\z", TokenType::UNKNOWN)));
}

TEST(LexerTest, LexInvalidBadUnicode) {
  Lexer lexer("\"\\u12\"");
  ++lexer;
  EXPECT_FALSE(lexer);
  EXPECT_FALSE(lexer.ok());
  EXPECT_THAT(lexer.error(),
              Eq(Lexer::Error(TokenType::STRING,
                              /*pos=*/1, "invalid control character: \\u12")));
  EXPECT_THAT(*lexer, Eq(Token("\\u12", TokenType::UNKNOWN)));
}

TEST(LexerTest, LexStringSimple) {
  Lexer lexer("\"hello\"");
  ++lexer;
  EXPECT_TRUE(lexer);
  EXPECT_TRUE(lexer.ok());
  EXPECT_THAT(*lexer, Eq(Token("hello", TokenType::STRING)));
}

TEST(LexerTest, LexStringEscapeChars) {
  Lexer lexer("\"hello\\nworld\"");
  ++lexer;
  EXPECT_TRUE(lexer);
  EXPECT_TRUE(lexer.ok());
  EXPECT_THAT(*lexer, Eq(Token("hello\nworld", TokenType::STRING)));
}

TEST(LexerTest, LexStringUnicode) {
  Lexer lexer("\"\\u0041\"");
  ++lexer;
  EXPECT_TRUE(lexer);
  EXPECT_TRUE(lexer.ok());
  EXPECT_THAT(*lexer, Eq(Token("\\u0041", TokenType::STRING)));
}

TEST(LexerTest, LexInvalidNumberDash) {
  Lexer lexer("-");
  ++lexer;
  EXPECT_FALSE(lexer);
  EXPECT_FALSE(lexer.ok());
  EXPECT_THAT(lexer.error(), Eq(Lexer::Error(TokenType::INTEGRAL, /*pos=*/0,
                                             "invalid integer: -")));
  EXPECT_THAT(*lexer, Eq(Token("-", TokenType::UNKNOWN)));
}

TEST(LexerTest, LexInvalidNumberLeadingZero) {
  Lexer lexer("01");
  ++lexer;
  EXPECT_FALSE(lexer);
  EXPECT_FALSE(lexer.ok());
  EXPECT_THAT(lexer.error(), Eq(Lexer::Error(TokenType::INTEGRAL, /*pos=*/0,
                                             "invalid integer: 01")));
  EXPECT_THAT(*lexer, Eq(Token("01", TokenType::UNKNOWN)));
}

TEST(LexerTest, LexInvalidNumberTrailingDecimal) {
  Lexer lexer("1.");
  ++lexer;
  EXPECT_FALSE(lexer);
  EXPECT_FALSE(lexer.ok());
  EXPECT_THAT(lexer.error(), Eq(Lexer::Error(TokenType::DOUBLE, /*pos=*/1,
                                             "invalid fraction: .")));
  EXPECT_THAT(*lexer, Eq(Token("1.", TokenType::UNKNOWN)));
}

TEST(LexerTest, LexInvalidNumberExponentOnly) {
  Lexer lexer("1e");
  ++lexer;
  EXPECT_FALSE(lexer);
  EXPECT_FALSE(lexer.ok());
  EXPECT_THAT(lexer.error(), Eq(Lexer::Error(TokenType::INTEGRAL, /*pos=*/1,
                                             "invalid exponent: e")));
  EXPECT_THAT(*lexer, Eq(Token("1e", TokenType::UNKNOWN)));
}

TEST(LexerTest, LexInvalidNumberExponentSign) {
  Lexer lexer("1e+");
  ++lexer;
  EXPECT_FALSE(lexer);
  EXPECT_FALSE(lexer.ok());
  EXPECT_THAT(lexer.error(), Eq(Lexer::Error(TokenType::INTEGRAL, /*pos=*/1,
                                             "invalid exponent: e+")));
  EXPECT_THAT(*lexer, Eq(Token("1e+", TokenType::UNKNOWN)));
}

TEST(LexerTest, LexNumberIntegral) {
  Lexer lexer("123");
  ++lexer;
  EXPECT_TRUE(lexer);
  EXPECT_TRUE(lexer.ok());
  EXPECT_THAT(*lexer, Eq(Token("123", TokenType::INTEGRAL)));
}

TEST(LexerTest, LexNumberDouble) {
  Lexer lexer("12.34");
  ++lexer;
  EXPECT_TRUE(lexer);
  EXPECT_TRUE(lexer.ok());
  EXPECT_THAT(*lexer, Eq(Token("12.34", TokenType::DOUBLE)));
}

TEST(LexerTest, LexPunctuation) {
  Lexer lexer("{}[],:");

  ++lexer;
  EXPECT_TRUE(lexer);
  EXPECT_TRUE(lexer.ok());
  EXPECT_THAT(*lexer, Eq(Token("{", TokenType::OBJECT_START)));

  ++lexer;
  EXPECT_TRUE(lexer);
  EXPECT_TRUE(lexer.ok());
  EXPECT_THAT(*lexer, Eq(Token("}", TokenType::OBJECT_END)));

  ++lexer;
  EXPECT_TRUE(lexer);
  EXPECT_TRUE(lexer.ok());
  EXPECT_THAT(*lexer, Eq(Token("[", TokenType::ARRAY_START)));

  ++lexer;
  EXPECT_TRUE(lexer);
  EXPECT_TRUE(lexer.ok());
  EXPECT_THAT(*lexer, Eq(Token("]", TokenType::ARRAY_END)));

  ++lexer;
  EXPECT_TRUE(lexer);
  EXPECT_TRUE(lexer.ok());
  EXPECT_THAT(*lexer, Eq(Token(",", TokenType::COMMA)));

  ++lexer;
  EXPECT_TRUE(lexer);
  EXPECT_TRUE(lexer.ok());
  EXPECT_THAT(*lexer, Eq(Token(":", TokenType::COLON)));
}

TEST(LexerTest, LexWhitespace) {
  Lexer lexer("   \n\t 123");
  ++lexer;
  EXPECT_TRUE(lexer);
  EXPECT_TRUE(lexer.ok());
  EXPECT_THAT(*lexer, Eq(Token("123", TokenType::INTEGRAL)));
}

TEST(LexerTest, LexUnknown) {
  Lexer lexer("@");
  ++lexer;
  EXPECT_FALSE(lexer);
  EXPECT_FALSE(lexer.ok());
  EXPECT_THAT(lexer.error(), Eq(Lexer::Error(TokenType::UNKNOWN, /*pos=*/0,
                                             "unknown token: @")));
  EXPECT_THAT(*lexer, Eq(Token("@", TokenType::UNKNOWN)));
}

TEST(LexerTest, IncrementOperator) {
  Lexer lexer("true false");

  ++lexer;
  EXPECT_TRUE(lexer);
  EXPECT_FALSE(lexer.eof());
  EXPECT_TRUE(lexer.ok());
  EXPECT_THAT(*lexer, Eq(Token("true", TokenType::BOOLEAN)));

  ++lexer;
  EXPECT_TRUE(lexer);
  EXPECT_FALSE(lexer.eof());
  EXPECT_TRUE(lexer.ok());
  EXPECT_THAT(*lexer, Eq(Token("false", TokenType::BOOLEAN)));

  ++lexer;
  EXPECT_FALSE(lexer);
  EXPECT_TRUE(lexer.eof());
}

TEST(LexerTest, EofOnEmptyInput) {
  Lexer lexer("");
  ++lexer;
  EXPECT_FALSE(lexer);
  EXPECT_TRUE(lexer.eof());
}

}  // namespace

}  // namespace syntax
}  // namespace json
}  // namespace warren
