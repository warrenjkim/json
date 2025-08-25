#include "warren/json/internal/parse/lexer.h"

#include "gtest/gtest.h"
#include "warren/json/internal/parse/token.h"

class LexerTest : public ::testing::Test {};

TEST_F(LexerTest, LexInvalidLogicalValues) {
  {  // incomplete null
    json::syntax::Lexer lexer("nul");
    json::syntax::Token token = lexer.next_token();
    EXPECT_EQ(token.type, json::syntax::TokenType::UNKNOWN);
    EXPECT_EQ(token.value, "nul");
  }
  {  // incomplete true
    json::syntax::Lexer lexer("tru");
    json::syntax::Token token = lexer.next_token();
    EXPECT_EQ(token.type, json::syntax::TokenType::UNKNOWN);
    EXPECT_EQ(token.value, "tru");
  }
  {  // incomplete false
    json::syntax::Lexer lexer("fals");
    json::syntax::Token token = lexer.next_token();
    EXPECT_EQ(token.type, json::syntax::TokenType::UNKNOWN);
    EXPECT_EQ(token.value, "fals");
  }
}

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

TEST_F(LexerTest, LexInvalidStrings) {
  {  // unterminated string
    json::syntax::Lexer lexer("\"hello");
    json::syntax::Token token = lexer.next_token();
    EXPECT_EQ(token.type, json::syntax::TokenType::UNKNOWN);
    EXPECT_EQ(token.value, "hello");
  }
  {  // bad escape char
    json::syntax::Lexer lexer("\"hello\\z\"");
    json::syntax::Token token = lexer.next_token();
    EXPECT_EQ(token.type, json::syntax::TokenType::UNKNOWN);
    EXPECT_EQ(token.value, "hello\\z");
  }
  {  // bad unicode
    json::syntax::Lexer lexer("\"\\u12\"");
    json::syntax::Token token = lexer.next_token();
    EXPECT_EQ(token.type, json::syntax::TokenType::UNKNOWN);
    EXPECT_EQ(token.value, "\\u12");
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
    EXPECT_EQ(token.value, "\\u0041");
  }
}

TEST_F(LexerTest, LexInvalidNumbers) {
  {  // -
    json::syntax::Lexer lexer("-");
    json::syntax::Token token = lexer.next_token();
    EXPECT_EQ(token.type, json::syntax::TokenType::UNKNOWN);
    EXPECT_EQ(token.value, "-");
  }
  {  // 01
    json::syntax::Lexer lexer("01");
    json::syntax::Token token = lexer.next_token();
    EXPECT_EQ(token.type, json::syntax::TokenType::UNKNOWN);
    EXPECT_EQ(token.value, "01");
  }
  {  // 1.
    json::syntax::Lexer lexer("1.");
    json::syntax::Token token = lexer.next_token();
    EXPECT_EQ(token.type, json::syntax::TokenType::UNKNOWN);
    EXPECT_EQ(token.value, "1.");
  }
  {  // 1e
    json::syntax::Lexer lexer("1e");
    json::syntax::Token token = lexer.next_token();
    EXPECT_EQ(token.type, json::syntax::TokenType::UNKNOWN);
    EXPECT_EQ(token.value, "1e");
  }
  {  // 1e+
    json::syntax::Lexer lexer("1e+");
    json::syntax::Token token = lexer.next_token();
    EXPECT_EQ(token.type, json::syntax::TokenType::UNKNOWN);
    EXPECT_EQ(token.value, "1e+");
  }
}

TEST_F(LexerTest, LexNumber) {
  {  // 123
    json::syntax::Lexer lexer("123");
    json::syntax::Token token = lexer.next_token();
    EXPECT_EQ(token.type, json::syntax::TokenType::NUMBER);
    EXPECT_EQ(token.value, "123");
  }
  {  // 12.34
    json::syntax::Lexer lexer("12.34");
    json::syntax::Token token = lexer.next_token();
    EXPECT_EQ(token.type, json::syntax::TokenType::NUMBER);
    EXPECT_EQ(token.value, "12.34");
  }
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
  json::syntax::Lexer lexer("{}[],:");
  for (size_t i = 0; i < inputs.size(); i++) {
    json::syntax::Token token = *(++lexer);
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
  EXPECT_FALSE(lexer.eof());
  EXPECT_EQ((*lexer).type, json::syntax::TokenType::BOOLEAN);
  EXPECT_EQ((*lexer).value, "true");

  ++lexer;
  EXPECT_FALSE(lexer.eof());
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
