#include "warren/internal/parse/lexer.h"

#include "gtest/gtest.h"
#include "warren/internal/parse/token.h"

class LexerTest : public ::testing::Test {};

TEST_F(LexerTest, LexNull) {
  json::syntax::Lexer lexer("null");
  json::syntax::Token token = lexer.next_token();
  EXPECT_EQ(token,
            json::syntax::Token("null", json::syntax::TokenType::JSON_NULL));
}

TEST_F(LexerTest, LexBoolean) {
  {  // true
    json::syntax::Lexer lexer("true");
    json::syntax::Token token = lexer.next_token();
    EXPECT_EQ(token.type, json::syntax::TokenType::BOOLEAN);
    EXPECT_EQ(token.value, "true");
  }
  {  // false
    json::syntax::Lexer lexer("false");
    json::syntax::Token token = lexer.next_token();
    EXPECT_EQ(token.type, json::syntax::TokenType::BOOLEAN);
    EXPECT_EQ(token.value, "false");
  }
}

TEST_F(LexerTest, LexString) {
  {  // simple
    json::syntax::Lexer lexer("\"hello\"");
    json::syntax::Token token = lexer.next_token();
    EXPECT_EQ(token.type, json::syntax::TokenType::STRING);
    EXPECT_EQ(token.value, "hello");
  }
  {  // escape chars
    json::syntax::Lexer lexer("\"hello\\nworld\"");
    json::syntax::Token token = lexer.next_token();
    EXPECT_EQ(token.value, "hello\nworld");
  }
  {  // unicode
    json::syntax::Lexer lexer("\"\\u0041\"");
    json::syntax::Token token = lexer.next_token();
    EXPECT_EQ(token.value, "A");
  }
}

TEST_F(LexerTest, LexNumber) {
  json::syntax::Lexer lexer("123");
  json::syntax::Token token = lexer.next_token();
  EXPECT_EQ(token.type, json::syntax::TokenType::NUMBER);
  EXPECT_EQ(token.value, "123");
}

TEST_F(LexerTest, LexPunctuation) {
  std::vector<json::syntax::Token> expected = {
      json::syntax::Token("{", json::syntax::TokenType::OBJECT_START),
      json::syntax::Token("}", json::syntax::TokenType::OBJECT_END),
      json::syntax::Token("[", json::syntax::TokenType::ARRAY_START),
      json::syntax::Token("]", json::syntax::TokenType::ARRAY_END),
      json::syntax::Token(",", json::syntax::TokenType::COMMA),
      json::syntax::Token(":", json::syntax::TokenType::COLON),
  };

  std::vector<std::string> inputs = {"{", "}", "[", "]", ",", ":"};

  for (size_t i = 0; i < inputs.size(); ++i) {
    json::syntax::Lexer lexer(inputs[i]);
    json::syntax::Token token = lexer.next_token();
    EXPECT_EQ(token, expected[i]);
  }
}

TEST_F(LexerTest, LexWhitespace) {
  json::syntax::Lexer lexer("   \n\t 123");
  json::syntax::Token token = lexer.next_token();
  EXPECT_EQ(token.type, json::syntax::TokenType::NUMBER);
  EXPECT_EQ(token.value, "123");
}

TEST_F(LexerTest, LexUnknown) {
  json::syntax::Lexer lexer("@");
  json::syntax::Token token = lexer.next_token();
  EXPECT_EQ(token.type, json::syntax::TokenType::UNKNOWN);
  EXPECT_EQ(token.value, "@");
}

TEST_F(LexerTest, IncrementOperator) {
  json::syntax::Lexer lexer("true false");

  ++lexer;
  ASSERT_FALSE(lexer.eof());
  EXPECT_EQ((*lexer).type, json::syntax::TokenType::BOOLEAN);
  EXPECT_EQ((*lexer).value, "true");

  ++lexer;
  ASSERT_FALSE(lexer.eof());
  EXPECT_EQ((*lexer).type, json::syntax::TokenType::BOOLEAN);
  EXPECT_EQ((*lexer).value, "false");

  ++lexer;
  EXPECT_TRUE(lexer.eof());
}

TEST_F(LexerTest, EofOnEmptyInput) {
  json::syntax::Lexer lexer("");
  ++lexer;
  EXPECT_TRUE(lexer.eof());
}
