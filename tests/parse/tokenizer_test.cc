#include "parse/tokenizer.h"

#include <stdio.h>

#include <boost/log/trivial.hpp>
#include <optional>
#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "parse/token.h"
#include "utils/logger.h"
#include "utils/typedefs.h"

class JsonTokenizerTest : public ::testing::Test {
 protected:
  void SetUp() override {
    json::utils::init_logging(boost::log::trivial::debug);
  }

  void assert_tokenization(const std::string_view input,
                           const std::vector<json::Token>& expected_tokens) {
    auto result = json::Tokenizer::tokenize(input);
    ASSERT_TRUE(result.has_value()) << "Failed to tokenize: " << input;
    json::utils::Queue<json::Token> actual_tokens = result.value();
    for (const json::Token& expected : expected_tokens) {
      ASSERT_TRUE(!actual_tokens.empty()) << "Fewer tokens than expected";
      const auto& actual = actual_tokens.front();
      ASSERT_EQ(expected, actual)
          << "Token mismatch. \nExpected: value='" + expected.value +
                 "', type=" + std::to_string(static_cast<int>(expected.type)) +
                 "\n" + "Actual: value='" + actual->value +
                 "', type=" + std::to_string(static_cast<int>(actual->type));
      actual_tokens.dequeue();
    }
    ASSERT_TRUE(actual_tokens.empty()) << "More tokens than expected";
  }

  void assert_invalid_input(const std::string_view input) {
    auto result = json::Tokenizer::tokenize(input);
    ASSERT_FALSE(result.has_value())
        << "Incorrectly tokenized invalid input: " << input;
  }
};

TEST_F(JsonTokenizerTest, ValidEmptyAndWhitespace) {
  assert_tokenization("", {});
  assert_tokenization(" [ ] ",
                      {json::Token("[", json::TokenType::ARRAY_START),
                       json::Token("]", json::TokenType::ARRAY_END),
                       json::Token("\0", json::TokenType::END_OF_JSON)});
  assert_tokenization("[\n  1,\n  2,\n  3\n]",
                      {json::Token("[", json::TokenType::ARRAY_START),
                       json::Token("1", json::TokenType::NUMBER),
                       json::Token(",", json::TokenType::COMMA),
                       json::Token("2", json::TokenType::NUMBER),
                       json::Token(",", json::TokenType::COMMA),
                       json::Token("3", json::TokenType::NUMBER),
                       json::Token("]", json::TokenType::ARRAY_END),
                       json::Token("\0", json::TokenType::END_OF_JSON)});
}

TEST_F(JsonTokenizerTest, ValidBasicStructures) {
  assert_tokenization("[]",

                      {json::Token("[", json::TokenType::ARRAY_START),
                       json::Token("]", json::TokenType::ARRAY_END),
                       json::Token("\0", json::TokenType::END_OF_JSON)});
  assert_tokenization("{}",

                      {json::Token("{", json::TokenType::OBJECT_START),
                       json::Token("}", json::TokenType::OBJECT_END),
                       json::Token("\0", json::TokenType::END_OF_JSON)});
  assert_tokenization("[{}]",

                      {json::Token("[", json::TokenType::ARRAY_START),
                       json::Token("{", json::TokenType::OBJECT_START),
                       json::Token("}", json::TokenType::OBJECT_END),
                       json::Token("]", json::TokenType::ARRAY_END),
                       json::Token("\0", json::TokenType::END_OF_JSON)});
  assert_tokenization("{\"\":\"\"}",
                      {json::Token("{", json::TokenType::OBJECT_START),
                       json::Token("\"", json::TokenType::QUOTE),
                       json::Token("", json::TokenType::STRING),
                       json::Token("\"", json::TokenType::QUOTE),
                       json::Token(":", json::TokenType::COLON),
                       json::Token("\"", json::TokenType::QUOTE),
                       json::Token("", json::TokenType::STRING),
                       json::Token("\"", json::TokenType::QUOTE),
                       json::Token("}", json::TokenType::OBJECT_END),
                       json::Token("\0", json::TokenType::END_OF_JSON)});
}

TEST_F(JsonTokenizerTest, ValidNumbers) {
  assert_tokenization("[0,42,-0,-42,3.14,-3.14,1e10,1e-10,1.23e+10,-1.23E-10]",
                      {json::Token("[", json::TokenType::ARRAY_START),
                       json::Token("0", json::TokenType::NUMBER),
                       json::Token(",", json::TokenType::COMMA),
                       json::Token("42", json::TokenType::NUMBER),
                       json::Token(",", json::TokenType::COMMA),
                       json::Token("-0", json::TokenType::NUMBER),
                       json::Token(",", json::TokenType::COMMA),
                       json::Token("-42", json::TokenType::NUMBER),
                       json::Token(",", json::TokenType::COMMA),
                       json::Token("3.14", json::TokenType::NUMBER),
                       json::Token(",", json::TokenType::COMMA),
                       json::Token("-3.14", json::TokenType::NUMBER),
                       json::Token(",", json::TokenType::COMMA),
                       json::Token("1e10", json::TokenType::NUMBER),
                       json::Token(",", json::TokenType::COMMA),
                       json::Token("1e-10", json::TokenType::NUMBER),
                       json::Token(",", json::TokenType::COMMA),
                       json::Token("1.23e+10", json::TokenType::NUMBER),
                       json::Token(",", json::TokenType::COMMA),
                       json::Token("-1.23E-10", json::TokenType::NUMBER),
                       json::Token("]", json::TokenType::ARRAY_END),
                       json::Token("\0", json::TokenType::END_OF_JSON)});

  assert_tokenization("[0e0,-0e-0,1e-999,1e+999,1e308]",
                      {json::Token("[", json::TokenType::ARRAY_START),
                       json::Token("0e0", json::TokenType::NUMBER),
                       json::Token(",", json::TokenType::COMMA),
                       json::Token("-0e-0", json::TokenType::NUMBER),
                       json::Token(",", json::TokenType::COMMA),
                       json::Token("1e-999", json::TokenType::NUMBER),
                       json::Token(",", json::TokenType::COMMA),
                       json::Token("1e+999", json::TokenType::NUMBER),
                       json::Token(",", json::TokenType::COMMA),
                       json::Token("1e308", json::TokenType::NUMBER),
                       json::Token("]", json::TokenType::ARRAY_END),
                       json::Token("\0", json::TokenType::END_OF_JSON)});

  assert_tokenization(
      "[0.0000000000000000000001,9999999999999999999999]",
      {json::Token("[", json::TokenType::ARRAY_START),
       json::Token("0.0000000000000000000001", json::TokenType::NUMBER),
       json::Token(",", json::TokenType::COMMA),
       json::Token("9999999999999999999999", json::TokenType::NUMBER),
       json::Token("]", json::TokenType::ARRAY_END),
       json::Token("\0", json::TokenType::END_OF_JSON)});
}

TEST_F(JsonTokenizerTest, ValidStrings) {
  assert_tokenization(
      "[\"Hello, world!\",\"Escaped \\\" "
      "quote\",\"\\u0041\",\"\\n\\t\\r\\b\\f\"]",
      {json::Token("[", json::TokenType::ARRAY_START),
       json::Token("\"", json::TokenType::QUOTE),
       json::Token("Hello, world!", json::TokenType::STRING),
       json::Token("\"", json::TokenType::QUOTE),
       json::Token(",", json::TokenType::COMMA),
       json::Token("\"", json::TokenType::QUOTE),
       json::Token("Escaped \\\" quote", json::TokenType::STRING),
       json::Token("\"", json::TokenType::QUOTE),
       json::Token(",", json::TokenType::COMMA),
       json::Token("\"", json::TokenType::QUOTE),
       json::Token("\\u0041", json::TokenType::STRING),
       json::Token("\"", json::TokenType::QUOTE),
       json::Token(",", json::TokenType::COMMA),
       json::Token("\"", json::TokenType::QUOTE),
       json::Token("\\n\\t\\r\\b\\f", json::TokenType::STRING),
       json::Token("\"", json::TokenType::QUOTE),
       json::Token("]", json::TokenType::ARRAY_END),
       json::Token("\0", json::TokenType::END_OF_JSON)});

  assert_tokenization(
      "[\"\\u0000\",\"\\\\\\\"\",\"line1\\nline2\\nline3\"]",
      {json::Token("[", json::TokenType::ARRAY_START),
       json::Token("\"", json::TokenType::QUOTE),
       json::Token("\\u0000", json::TokenType::STRING),
       json::Token("\"", json::TokenType::QUOTE),
       json::Token(",", json::TokenType::COMMA),
       json::Token("\"", json::TokenType::QUOTE),
       json::Token("\\\\\\\"", json::TokenType::STRING),
       json::Token("\"", json::TokenType::QUOTE),
       json::Token(",", json::TokenType::COMMA),
       json::Token("\"", json::TokenType::QUOTE),
       json::Token("line1\\nline2\\nline3", json::TokenType::STRING),
       json::Token("\"", json::TokenType::QUOTE),
       json::Token("]", json::TokenType::ARRAY_END),
       json::Token("\0", json::TokenType::END_OF_JSON)});
}

TEST_F(JsonTokenizerTest, ValidUnicode) {
  assert_tokenization("[\"\\u0001\",\"\\uFFFF\",\"😀\",\"🌈\",\"🚀\"]",
                      {json::Token("[", json::TokenType::ARRAY_START),
                       json::Token("\"", json::TokenType::QUOTE),
                       json::Token("\\u0001", json::TokenType::STRING),
                       json::Token("\"", json::TokenType::QUOTE),
                       json::Token(",", json::TokenType::COMMA),
                       json::Token("\"", json::TokenType::QUOTE),
                       json::Token("\\uFFFF", json::TokenType::STRING),
                       json::Token("\"", json::TokenType::QUOTE),
                       json::Token(",", json::TokenType::COMMA),
                       json::Token("\"", json::TokenType::QUOTE),
                       json::Token("😀", json::TokenType::STRING),
                       json::Token("\"", json::TokenType::QUOTE),
                       json::Token(",", json::TokenType::COMMA),
                       json::Token("\"", json::TokenType::QUOTE),
                       json::Token("🌈", json::TokenType::STRING),
                       json::Token("\"", json::TokenType::QUOTE),
                       json::Token(",", json::TokenType::COMMA),
                       json::Token("\"", json::TokenType::QUOTE),
                       json::Token("🚀", json::TokenType::STRING),
                       json::Token("\"", json::TokenType::QUOTE),
                       json::Token("]", json::TokenType::ARRAY_END),
                       json::Token("\0", json::TokenType::END_OF_JSON)});

  assert_tokenization("{\"\\u00A9\":\"copyright\"}",
                      {json::Token("{", json::TokenType::OBJECT_START),
                       json::Token("\"", json::TokenType::QUOTE),
                       json::Token("\\u00A9", json::TokenType::STRING),
                       json::Token("\"", json::TokenType::QUOTE),
                       json::Token(":", json::TokenType::COLON),
                       json::Token("\"", json::TokenType::QUOTE),
                       json::Token("copyright", json::TokenType::STRING),
                       json::Token("\"", json::TokenType::QUOTE),
                       json::Token("}", json::TokenType::OBJECT_END),
                       json::Token("\0", json::TokenType::END_OF_JSON)});
}

TEST_F(JsonTokenizerTest, ValidArrays) {
  assert_tokenization("[1,\"two\",3.14,true,null,{},[],-1]",
                      {json::Token("[", json::TokenType::ARRAY_START),
                       json::Token("1", json::TokenType::NUMBER),
                       json::Token(",", json::TokenType::COMMA),
                       json::Token("\"", json::TokenType::QUOTE),
                       json::Token("two", json::TokenType::STRING),
                       json::Token("\"", json::TokenType::QUOTE),
                       json::Token(",", json::TokenType::COMMA),
                       json::Token("3.14", json::TokenType::NUMBER),
                       json::Token(",", json::TokenType::COMMA),
                       json::Token("true", json::TokenType::BOOLEAN),
                       json::Token(",", json::TokenType::COMMA),
                       json::Token("null", json::TokenType::JSON_NULL),
                       json::Token(",", json::TokenType::COMMA),
                       json::Token("{", json::TokenType::OBJECT_START),
                       json::Token("}", json::TokenType::OBJECT_END),
                       json::Token(",", json::TokenType::COMMA),
                       json::Token("[", json::TokenType::ARRAY_START),
                       json::Token("]", json::TokenType::ARRAY_END),
                       json::Token(",", json::TokenType::COMMA),
                       json::Token("-1", json::TokenType::NUMBER),
                       json::Token("]", json::TokenType::ARRAY_END),
                       json::Token("\0", json::TokenType::END_OF_JSON)});
}

TEST_F(JsonTokenizerTest, ValidObjects) {
  assert_tokenization("{\"key\":\"value\",\"nested\":{\"array\":[1,2,3]}}",
                      {json::Token("{", json::TokenType::OBJECT_START),
                       json::Token("\"", json::TokenType::QUOTE),
                       json::Token("key", json::TokenType::STRING),
                       json::Token("\"", json::TokenType::QUOTE),
                       json::Token(":", json::TokenType::COLON),
                       json::Token("\"", json::TokenType::QUOTE),
                       json::Token("value", json::TokenType::STRING),
                       json::Token("\"", json::TokenType::QUOTE),
                       json::Token(",", json::TokenType::COMMA),
                       json::Token("\"", json::TokenType::QUOTE),
                       json::Token("nested", json::TokenType::STRING),
                       json::Token("\"", json::TokenType::QUOTE),
                       json::Token(":", json::TokenType::COLON),
                       json::Token("{", json::TokenType::OBJECT_START),
                       json::Token("\"", json::TokenType::QUOTE),
                       json::Token("array", json::TokenType::STRING),
                       json::Token("\"", json::TokenType::QUOTE),
                       json::Token(":", json::TokenType::COLON),
                       json::Token("[", json::TokenType::ARRAY_START),
                       json::Token("1", json::TokenType::NUMBER),
                       json::Token(",", json::TokenType::COMMA),
                       json::Token("2", json::TokenType::NUMBER),
                       json::Token(",", json::TokenType::COMMA),
                       json::Token("3", json::TokenType::NUMBER),
                       json::Token("]", json::TokenType::ARRAY_END),
                       json::Token("}", json::TokenType::OBJECT_END),
                       json::Token("}", json::TokenType::OBJECT_END),
                       json::Token("\0", json::TokenType::END_OF_JSON)});
}

TEST_F(JsonTokenizerTest, ValidNestedStructures) {
  assert_tokenization("[[[[]]]]",
                      {json::Token("[", json::TokenType::ARRAY_START),
                       json::Token("[", json::TokenType::ARRAY_START),
                       json::Token("[", json::TokenType::ARRAY_START),
                       json::Token("[", json::TokenType::ARRAY_START),
                       json::Token("]", json::TokenType::ARRAY_END),
                       json::Token("]", json::TokenType::ARRAY_END),
                       json::Token("]", json::TokenType::ARRAY_END),
                       json::Token("]", json::TokenType::ARRAY_END),
                       json::Token("\0", json::TokenType::END_OF_JSON)});

  assert_tokenization("{\"a\":{\"b\":{\"c\":{}}}}",
                      {json::Token("{", json::TokenType::OBJECT_START),
                       json::Token("\"", json::TokenType::QUOTE),
                       json::Token("a", json::TokenType::STRING),
                       json::Token("\"", json::TokenType::QUOTE),
                       json::Token(":", json::TokenType::COLON),
                       json::Token("{", json::TokenType::OBJECT_START),
                       json::Token("\"", json::TokenType::QUOTE),
                       json::Token("b", json::TokenType::STRING),
                       json::Token("\"", json::TokenType::QUOTE),
                       json::Token(":", json::TokenType::COLON),
                       json::Token("{", json::TokenType::OBJECT_START),
                       json::Token("\"", json::TokenType::QUOTE),
                       json::Token("c", json::TokenType::STRING),
                       json::Token("\"", json::TokenType::QUOTE),
                       json::Token(":", json::TokenType::COLON),
                       json::Token("{", json::TokenType::OBJECT_START),
                       json::Token("}", json::TokenType::OBJECT_END),
                       json::Token("}", json::TokenType::OBJECT_END),
                       json::Token("}", json::TokenType::OBJECT_END),
                       json::Token("}", json::TokenType::OBJECT_END),
                       json::Token("\0", json::TokenType::END_OF_JSON)});
}

TEST_F(JsonTokenizerTest, ValidLogicalValues) {
  assert_tokenization("[true]",
                      {json::Token("[", json::TokenType::ARRAY_START),
                       json::Token("true", json::TokenType::BOOLEAN),
                       json::Token("]", json::TokenType::ARRAY_END),
                       json::Token("\0", json::TokenType::END_OF_JSON)});

  assert_tokenization("[false]",
                      {json::Token("[", json::TokenType::ARRAY_START),
                       json::Token("false", json::TokenType::BOOLEAN),
                       json::Token("]", json::TokenType::ARRAY_END),
                       json::Token("\0", json::TokenType::END_OF_JSON)});

  assert_tokenization("[null]",
                      {json::Token("[", json::TokenType::ARRAY_START),
                       json::Token("null", json::TokenType::JSON_NULL),
                       json::Token("]", json::TokenType::ARRAY_END),
                       json::Token("\0", json::TokenType::END_OF_JSON)});
}

TEST_F(JsonTokenizerTest, ValidComplexJson) {
  assert_tokenization(
      "{\"menu\":{\"id\":\"file\",\"value\":\"File\",\"popup\":{\"menuitem\":["
      "{\"value\":\"New\",\"onclick\":\"CreateNewDoc()\"},"
      "{\"value\":\"Open\",\"onclick\":\"OpenDoc()\"},"
      "{\"value\":\"Close\",\"onclick\":\"CloseDoc()\"}]}}}",
      {json::Token("{", json::TokenType::OBJECT_START),
       json::Token("\"", json::TokenType::QUOTE),
       json::Token("menu", json::TokenType::STRING),
       json::Token("\"", json::TokenType::QUOTE),
       json::Token(":", json::TokenType::COLON),
       json::Token("{", json::TokenType::OBJECT_START),
       json::Token("\"", json::TokenType::QUOTE),
       json::Token("id", json::TokenType::STRING),
       json::Token("\"", json::TokenType::QUOTE),
       json::Token(":", json::TokenType::COLON),
       json::Token("\"", json::TokenType::QUOTE),
       json::Token("file", json::TokenType::STRING),
       json::Token("\"", json::TokenType::QUOTE),
       json::Token(",", json::TokenType::COMMA),
       json::Token("\"", json::TokenType::QUOTE),
       json::Token("value", json::TokenType::STRING),
       json::Token("\"", json::TokenType::QUOTE),
       json::Token(":", json::TokenType::COLON),
       json::Token("\"", json::TokenType::QUOTE),
       json::Token("File", json::TokenType::STRING),
       json::Token("\"", json::TokenType::QUOTE),
       json::Token(",", json::TokenType::COMMA),
       json::Token("\"", json::TokenType::QUOTE),
       json::Token("popup", json::TokenType::STRING),
       json::Token("\"", json::TokenType::QUOTE),
       json::Token(":", json::TokenType::COLON),
       json::Token("{", json::TokenType::OBJECT_START),
       json::Token("\"", json::TokenType::QUOTE),
       json::Token("menuitem", json::TokenType::STRING),
       json::Token("\"", json::TokenType::QUOTE),
       json::Token(":", json::TokenType::COLON),
       json::Token("[", json::TokenType::ARRAY_START),
       json::Token("{", json::TokenType::OBJECT_START),
       json::Token("\"", json::TokenType::QUOTE),
       json::Token("value", json::TokenType::STRING),
       json::Token("\"", json::TokenType::QUOTE),
       json::Token(":", json::TokenType::COLON),
       json::Token("\"", json::TokenType::QUOTE),
       json::Token("New", json::TokenType::STRING),
       json::Token("\"", json::TokenType::QUOTE),
       json::Token(",", json::TokenType::COMMA),
       json::Token("\"", json::TokenType::QUOTE),
       json::Token("onclick", json::TokenType::STRING),
       json::Token("\"", json::TokenType::QUOTE),
       json::Token(":", json::TokenType::COLON),
       json::Token("\"", json::TokenType::QUOTE),
       json::Token("CreateNewDoc()", json::TokenType::STRING),
       json::Token("\"", json::TokenType::QUOTE),
       json::Token("}", json::TokenType::OBJECT_END),
       json::Token(",", json::TokenType::COMMA),
       json::Token("{", json::TokenType::OBJECT_START),
       json::Token("\"", json::TokenType::QUOTE),
       json::Token("value", json::TokenType::STRING),
       json::Token("\"", json::TokenType::QUOTE),
       json::Token(":", json::TokenType::COLON),
       json::Token("\"", json::TokenType::QUOTE),
       json::Token("Open", json::TokenType::STRING),
       json::Token("\"", json::TokenType::QUOTE),
       json::Token(",", json::TokenType::COMMA),
       json::Token("\"", json::TokenType::QUOTE),
       json::Token("onclick", json::TokenType::STRING),
       json::Token("\"", json::TokenType::QUOTE),
       json::Token(":", json::TokenType::COLON),
       json::Token("\"", json::TokenType::QUOTE),
       json::Token("OpenDoc()", json::TokenType::STRING),
       json::Token("\"", json::TokenType::QUOTE),
       json::Token("}", json::TokenType::OBJECT_END),
       json::Token(",", json::TokenType::COMMA),
       json::Token("{", json::TokenType::OBJECT_START),
       json::Token("\"", json::TokenType::QUOTE),
       json::Token("value", json::TokenType::STRING),
       json::Token("\"", json::TokenType::QUOTE),
       json::Token(":", json::TokenType::COLON),
       json::Token("\"", json::TokenType::QUOTE),
       json::Token("Close", json::TokenType::STRING),
       json::Token("\"", json::TokenType::QUOTE),
       json::Token(",", json::TokenType::COMMA),
       json::Token("\"", json::TokenType::QUOTE),
       json::Token("onclick", json::TokenType::STRING),
       json::Token("\"", json::TokenType::QUOTE),
       json::Token(":", json::TokenType::COLON),
       json::Token("\"", json::TokenType::QUOTE),
       json::Token("CloseDoc()", json::TokenType::STRING),
       json::Token("\"", json::TokenType::QUOTE),
       json::Token("}", json::TokenType::OBJECT_END),
       json::Token("]", json::TokenType::ARRAY_END),
       json::Token("}", json::TokenType::OBJECT_END),
       json::Token("}", json::TokenType::OBJECT_END),
       json::Token("}", json::TokenType::OBJECT_END),
       json::Token("\0", json::TokenType::END_OF_JSON)});
}

TEST_F(JsonTokenizerTest, InvalidExtraTokens) {
  assert_invalid_input("{}}");
  assert_invalid_input("{{");
  assert_invalid_input("{");
  assert_invalid_input("{\"\":1,");
  assert_invalid_input("{\"\":1{");
  assert_invalid_input("{ ");
  assert_invalid_input("[]]");
  assert_invalid_input("[[");
  assert_invalid_input("[ ");
  assert_invalid_input("[10, ");
}

TEST_F(JsonTokenizerTest, InvalidLogicalValues) {
  assert_invalid_input("[treu]");
  assert_invalid_input("[fals]");
  assert_invalid_input("[nul]");
}

TEST_F(JsonTokenizerTest, InvalidControlCharacters) {
  assert_invalid_input("[\"\\u123k5\"]");
  assert_invalid_input("[\"\\u");
  assert_invalid_input("[\"\\u123");
  assert_invalid_input("[\"\\");
  assert_invalid_input("[\"\\ \"]");
}

TEST_F(JsonTokenizerTest, InvalidNumbers) {
  assert_invalid_input("[.]");
  assert_invalid_input("[+1]");
  assert_invalid_input("[01]");
  assert_invalid_input("[1.]");
  assert_invalid_input("[.1]");
  assert_invalid_input("[1e]");
  assert_invalid_input("[1e+]");
  assert_invalid_input("[1e-]");
  assert_invalid_input("[1e1.0]");
  assert_invalid_input("[-]");
  assert_invalid_input("[--1]");
  assert_invalid_input("[1-]");
  assert_invalid_input("[1.2.3]");
  assert_invalid_input("[1,000]");
  assert_invalid_input("[Infinity]");
  assert_invalid_input("[NaN]");
  assert_invalid_input("[1a]");
  assert_invalid_input("[0x123]");
  assert_invalid_input("[0b1010]");
  assert_invalid_input("[0o123]");
  assert_invalid_input("[1_000]");
  assert_invalid_input("[１２３]");
}

TEST_F(JsonTokenizerTest, InvalidStructures) {
  assert_invalid_input("[1,2,]");
  assert_invalid_input("{\"a\":1,}");
  assert_invalid_input("[1,2");
  assert_invalid_input("{\"a\":1");
  assert_invalid_input("\"unclosed string");
  assert_invalid_input("{a:1}");
  assert_invalid_input("[,]");
  assert_invalid_input("{,}");
  assert_invalid_input("{\"a\":}");
  assert_invalid_input("{:1}");
}

TEST_F(JsonTokenizerTest, InvalidKeyValues) {
  assert_invalid_input("{\"\"");
  assert_invalid_input("{\"\"a");
  assert_invalid_input("{\"\":");
}
