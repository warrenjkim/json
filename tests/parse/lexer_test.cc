#include "warren/json/internal/parse/lexer.h"

#include "gtest/gtest.h"
#include "warren/json/internal/parse/token.h"

namespace json {
namespace syntax {
namespace {

TEST(LexerTest, LexInvalidNull) {
  Lexer lexer("nul");
  ++lexer;
  EXPECT_FALSE(lexer);
  EXPECT_TRUE(lexer.has_error());
  Lexer::Error error = lexer.error();
  EXPECT_EQ(error.expected, TokenType::JSON_NULL);
  EXPECT_EQ(error.pos, 0);
  EXPECT_EQ(*lexer, Token("nul", TokenType::UNKNOWN));
}

TEST(LexerTest, LexInvalidTrue) {
  Lexer lexer("tru");
  Token token = lexer.next_token();
  EXPECT_EQ(token.type, TokenType::UNKNOWN);
  EXPECT_EQ(token.value, "tru");
}

TEST(LexerTest, LexInvalidFalse) {
  Lexer lexer("fals");
  Token token = lexer.next_token();
  EXPECT_EQ(token.type, TokenType::UNKNOWN);
  EXPECT_EQ(token.value, "fals");
}

TEST(LexerTest, LexNull) {
  Lexer lexer("null");
  Token token = lexer.next_token();
  EXPECT_EQ(token, Token("null", TokenType::JSON_NULL));
}

TEST(LexerTest, LexBoolean) {
  {  // true
    Lexer lexer("true");
    Token token = lexer.next_token();
    EXPECT_EQ(token.type, TokenType::BOOLEAN);
    EXPECT_EQ(token.value, "true");
  }
  {  // false
    Lexer lexer("false");
    Token token = lexer.next_token();
    EXPECT_EQ(token.type, TokenType::BOOLEAN);
    EXPECT_EQ(token.value, "false");
  }
}

TEST(LexerTest, LexInvalidStrings) {
  {  // unterminated string
    Lexer lexer("\"hello");
    Token token = lexer.next_token();
    EXPECT_EQ(token.type, TokenType::UNKNOWN);
    EXPECT_EQ(token.value, "hello");
  }
  {  // bad escape char
    Lexer lexer("\"hello\\z\"");
    Token token = lexer.next_token();
    EXPECT_EQ(token.type, TokenType::UNKNOWN);
    EXPECT_EQ(token.value, "hello\\z");
  }
  {  // bad unicode
    Lexer lexer("\"\\u12\"");
    Token token = lexer.next_token();
    EXPECT_EQ(token.type, TokenType::UNKNOWN);
    EXPECT_EQ(token.value, "\\u12");
  }
}

TEST(LexerTest, LexString) {
  {  // simple
    Lexer lexer("\"hello\"");
    Token token = lexer.next_token();
    EXPECT_EQ(token.type, TokenType::STRING);
    EXPECT_EQ(token.value, "hello");
  }
  {  // escape chars
    Lexer lexer("\"hello\\nworld\"");
    Token token = lexer.next_token();
    EXPECT_EQ(token.value, "hello\nworld");
  }
  {  // unicode
    Lexer lexer("\"\\u0041\"");
    Token token = lexer.next_token();
    EXPECT_EQ(token.value, "\\u0041");
  }
}

TEST(LexerTest, LexInvalidNumbers) {
  {  // -
    Lexer lexer("-");
    Token token = lexer.next_token();
    EXPECT_EQ(token.type, TokenType::UNKNOWN);
    EXPECT_EQ(token.value, "-");
  }
  {  // 01
    Lexer lexer("01");
    Token token = lexer.next_token();
    EXPECT_EQ(token.type, TokenType::UNKNOWN);
    EXPECT_EQ(token.value, "01");
  }
  {  // 1.
    Lexer lexer("1.");
    Token token = lexer.next_token();
    EXPECT_EQ(token.type, TokenType::UNKNOWN);
    EXPECT_EQ(token.value, "1.");
  }
  {  // 1e
    Lexer lexer("1e");
    Token token = lexer.next_token();
    EXPECT_EQ(token.type, TokenType::UNKNOWN);
    EXPECT_EQ(token.value, "1e");
  }
  {  // 1e+
    Lexer lexer("1e+");
    Token token = lexer.next_token();
    EXPECT_EQ(token.type, TokenType::UNKNOWN);
    EXPECT_EQ(token.value, "1e+");
  }
}

TEST(LexerTest, LexNumber) {
  {  // 123
    Lexer lexer("123");
    Token token = lexer.next_token();
    EXPECT_EQ(token.type, TokenType::INTEGRAL);
    EXPECT_EQ(token.value, "123");
  }
  {  // 12.34
    Lexer lexer("12.34");
    Token token = lexer.next_token();
    EXPECT_EQ(token.type, TokenType::DOUBLE);
    EXPECT_EQ(token.value, "12.34");
  }
}

TEST(LexerTest, LexPunctuation) {
  std::vector<Token> expected = {
      Token("{", TokenType::OBJECT_START), Token("}", TokenType::OBJECT_END),
      Token("[", TokenType::ARRAY_START),  Token("]", TokenType::ARRAY_END),
      Token(",", TokenType::COMMA),        Token(":", TokenType::COLON),
  };

  std::vector<std::string> inputs = {"{", "}", "[", "]", ",", ":"};
  Lexer lexer("{}[],:");
  for (size_t i = 0; i < inputs.size(); i++) {
    Token token = *(++lexer);
    EXPECT_EQ(token, expected[i]);
  }
}

TEST(LexerTest, LexWhitespace) {
  Lexer lexer("   \n\t 123");
  Token token = lexer.next_token();
  EXPECT_EQ(token.type, TokenType::INTEGRAL);
  EXPECT_EQ(token.value, "123");
}

TEST(LexerTest, LexUnknown) {
  Lexer lexer("@");
  Token token = lexer.next_token();
  EXPECT_EQ(token.type, TokenType::UNKNOWN);
  EXPECT_EQ(token.value, "@");
}

TEST(LexerTest, IncrementOperator) {
  Lexer lexer("true false");

  ++lexer;
  EXPECT_FALSE(lexer.eof());
  EXPECT_EQ((*lexer).type, TokenType::BOOLEAN);
  EXPECT_EQ((*lexer).value, "true");

  ++lexer;
  EXPECT_FALSE(lexer.eof());
  EXPECT_EQ((*lexer).type, TokenType::BOOLEAN);
  EXPECT_EQ((*lexer).value, "false");

  ++lexer;
  EXPECT_TRUE(lexer.eof());
}

TEST(LexerTest, EofOnEmptyInput) {
  Lexer lexer("");
  ++lexer;
  EXPECT_TRUE(lexer.eof());
}

}  // namespace
}  // namespace syntax
}  // namespace json
