#include "parse/parser.h"

#include <gtest/gtest.h>

#include <boost/log/trivial.hpp>
#include <cstdint>
#include <string_view>

#include "nodes/array_node.h"
#include "nodes/boolean_node.h"
#include "nodes/key_value_node.h"
#include "nodes/node.h"
#include "nodes/null_node.h"
#include "nodes/number_node.h"
#include "nodes/object_node.h"
#include "nodes/string_node.h"
#include "parse/token.h"
#include "utils/logger.h"
#include "visitors/visitor.h"

class JsonParserTest : public ::testing::Test {
 protected:
  void SetUp() override { utils::init_logging(boost::log::trivial::debug); }
  void assert_parse(const std::string_view input, json::Node* expected_ast) {
    std::unique_ptr<json::Node> result(json::Parser::parse(input));
    ASSERT_TRUE(result) << "Parser returned nullptr for valid input";
    ASSERT_EQ(*result, *expected_ast)
        << "Parsed result does not match expected AST";
    delete expected_ast;
  }

  void assert_parse_failure(const std::string_view input) {
    std::unique_ptr<json::Node> result(json::Parser::parse(input));
    ASSERT_FALSE(result) << "Parser did not fail for invalid input";
  }

  void assert_parse_failure(std::queue<json::Token> input) {
    std::unique_ptr<json::Node> result(json::Parser::parse(input));
    ASSERT_FALSE(result) << "Parser did not fail for invalid input";
  }

  std::queue<json::Token> make_token_queue(
      std::initializer_list<json::Token> tokens) {
    return std::queue<json::Token>(std::deque<json::Token>(tokens));
  }
};

TEST_F(JsonParserTest, EmptyObject) {
  assert_parse("{}", new json::ObjectNode());
}

TEST_F(JsonParserTest, SimpleObject) {
  json::ObjectNode* obj = new json::ObjectNode();
  obj->add(new json::KeyValueNode("key", new json::StringNode("value")));
  assert_parse("{\"key\": \"value\"}", obj);
}

TEST_F(JsonParserTest, EmptyArray) {
  assert_parse("[]", new json::ArrayNode());
}

TEST_F(JsonParserTest, SimpleArray) {
  json::ArrayNode* arr = new json::ArrayNode();
  arr->add(new json::NumberNode(1));
  arr->add(new json::NumberNode(2));
  arr->add(new json::NumberNode(3));
  assert_parse("[1, 2, 3]", arr);
}

TEST_F(JsonParserTest, LogicalValues) {
  json::ArrayNode* arr = new json::ArrayNode();
  arr->add(new json::BooleanNode(true));
  arr->add(new json::BooleanNode(false));
  arr->add(new json::NullNode());
  assert_parse("[true, false, null]", arr);
}

TEST_F(JsonParserTest, NullValue) {
  json::ObjectNode* obj = new json::ObjectNode();
  obj->add(new json::KeyValueNode("key", new json::NullNode()));
  assert_parse("{\"key\": null}", obj);

  json::ArrayNode* arr = new json::ArrayNode();
  arr->add(new json::NullNode());
  assert_parse("[null]", arr);
}

TEST_F(JsonParserTest, IntegerNumbers) {
  json::ArrayNode* arr = new json::ArrayNode();
  arr->add(new json::NumberNode(0));
  arr->add(new json::NumberNode(42));
  arr->add(new json::NumberNode(0));
  arr->add(new json::NumberNode(-42));
  assert_parse("[0,42,-0,-42]", arr);
}

TEST_F(JsonParserTest, FloatingPointNumbers) {
  json::ArrayNode* arr = new json::ArrayNode();
  arr->add(new json::NumberNode(3.14));
  arr->add(new json::NumberNode(-3.14));
  assert_parse("[3.14,-3.14]", arr);
}

TEST_F(JsonParserTest, ScientificNotationNumbers) {
  json::ArrayNode* arr = new json::ArrayNode();
  arr->add(new json::NumberNode(static_cast<int64_t>(1e10)));
  arr->add(new json::NumberNode(1e-10));
  arr->add(new json::NumberNode(1.23e10));
  arr->add(new json::NumberNode(-1.23e-10));
  assert_parse("[1e10,1e-10,1.23e+10,-1.23E-10]", arr);
}

TEST_F(JsonParserTest, ZeroWithExponent) {
  json::ArrayNode* arr = new json::ArrayNode();
  arr->add(new json::NumberNode(0));  // 0e0
  arr->add(new json::NumberNode(0));  // -0e-0
  assert_parse("[0e0,-0e-0]", arr);
}

TEST_F(JsonParserTest, LargeExponents) {
  json::ArrayNode* arr = new json::ArrayNode();
  arr->add(new json::NumberNode(1e-123));
  arr->add(new json::NumberNode(1e123));
  arr->add(new json::NumberNode(1e308));
  assert_parse("[1e-123,1e+123,1e308]", arr);
}

TEST_F(JsonParserTest, VerySmallAndLargeNumbers) {
  json::ArrayNode* arr = new json::ArrayNode();
  arr->add(new json::NumberNode(0.0000000000000000000001));
  arr->add(new json::NumberNode(9999999999999999999999.0));
  assert_parse("[0.0000000000000000000001,9999999999999999999999]", arr);
}

TEST_F(JsonParserTest, SimpleString) {
  json::ArrayNode* arr = new json::ArrayNode();
  arr->add(new json::StringNode("Hello, World!"));
  assert_parse("[\"Hello, World!\"]", arr);
}

TEST_F(JsonParserTest, EscapedString) {
  json::ArrayNode* arr = new json::ArrayNode();
  arr->add(new json::StringNode("Escaped \\\"Quote\\\""));
  assert_parse("[\"Escaped \\\"Quote\\\"\"]", arr);
}

TEST_F(JsonParserTest, ComplexStructure) {
  json::ObjectNode* obj = new json::ObjectNode();
  json::ObjectNode* inner_obj = new json::ObjectNode();
  inner_obj->add(new json::KeyValueNode("first", new json::StringNode("John")));
  inner_obj->add(new json::KeyValueNode("last", new json::StringNode("Doe")));
  obj->add(new json::KeyValueNode("name", inner_obj));
  obj->add(new json::KeyValueNode("age", new json::NumberNode(30)));
  obj->add(new json::KeyValueNode("isStudent", new json::BooleanNode(false)));

  json::ArrayNode* hobbies = new json::ArrayNode();
  hobbies->add(new json::StringNode("reading"));
  hobbies->add(new json::StringNode("cycling"));

  obj->add(new json::KeyValueNode("hobbies", hobbies));
  obj->add(new json::KeyValueNode("address", new json::NullNode()));

  assert_parse(R"({
    "name": {
      "first": "John",
      "last": "Doe"
    },
    "age": 30,
    "isStudent": false,
    "hobbies": ["reading", "cycling"],
    "address": null
  })",
               obj);
}

TEST_F(JsonParserTest, InvalidJsonBasic) {
  assert_parse_failure("{\"key\": }");
  assert_parse_failure("[1, 2, ]");
  assert_parse_failure("\"unclosed string");
  assert_parse_failure("invalid");
}

TEST_F(JsonParserTest, InvalidJsonObjects) {
  assert_parse_failure("{key: \"value\"}");       // Missing quotes around key
  assert_parse_failure("{\"key\" \"value\"}");    // Missing colon
  assert_parse_failure("{\"key\":}");             // Missing value
  assert_parse_failure("{\"key\": \"value\",}");  // Trailing comma
  assert_parse_failure(
      "{\"key\": \"value\" \"key2\": \"value2\"}");  // Missing comma between
                                                     // key-value pairs
}

TEST_F(JsonParserTest, InvalidJsonArrays) {
  assert_parse_failure("[,]");       // Empty element
  assert_parse_failure("[1 2]");     // Missing comma
  assert_parse_failure("[1, , 2]");  // Extra comma
  assert_parse_failure("[]]");       // Extra closing bracket
  assert_parse_failure("[[1, 2]");   // Unclosed array
}

TEST_F(JsonParserTest, InvalidJsonNumbers) {
  assert_parse_failure("123.");  // Trailing decimal point
  assert_parse_failure(".123");  // Leading decimal point
  assert_parse_failure("0123");  // Leading zeros
  assert_parse_failure("1e");    // Incomplete exponent
  assert_parse_failure("1e+");   // Incomplete exponent
  assert_parse_failure("1E-");   // Incomplete exponent
}

TEST_F(JsonParserTest, InvalidJsonStrings) {
  assert_parse_failure("\"escape character: \\\"");  // Incomplete escape
  assert_parse_failure("\"newline\ncharacter\"");    // Unescaped newline
  assert_parse_failure("\"tab\tcharacter\"");        // Unescaped tab
  assert_parse_failure("\"\u0123\"");                // Invalid Unicode escape
}

TEST_F(JsonParserTest, InvalidJsonBooleanAndNull) {
  assert_parse_failure("True");        // Capitalized true
  assert_parse_failure("False");       // Capitalized false
  assert_parse_failure("Null");        // Capitalized null
  assert_parse_failure("true false");  // Multiple values without container
}

TEST_F(JsonParserTest, InvalidJsonNestedStructures) {
  assert_parse_failure("{\"key\": [1, 2,]}");  // Trailing comma in nested array
  assert_parse_failure(
      "[{\"key\": \"value\",}]");  // Trailing comma in nested object
  assert_parse_failure(
      "{\"key\": [1, {\"inner\": 2]}");  // Unclosed nested structure
}

TEST_F(JsonParserTest, InvalidJsonWhitespaceIssues) {
  assert_parse_failure(" ");       // Empty input (only whitespace)
  assert_parse_failure("\t\n\r");  // Only whitespace characters
}

TEST_F(JsonParserTest, InvalidJsonComments) {
  assert_parse_failure("// This is a comment");
  assert_parse_failure("{\"key\": \"value\"} // comment");
  assert_parse_failure("/* Multi-line\ncomment */");
}

TEST_F(JsonParserTest, InvalidJsonControlCharacters) {
  assert_parse_failure("\x01");      // ASCII control character
  assert_parse_failure("\xC3\x28");  // Invalid UTF-8 sequence
}

TEST_F(JsonParserTest, InvalidJsonMultipleTopLevelValues) {
  assert_parse_failure("{} []");          // Multiple JSON values
  assert_parse_failure("\"string\" 42");  // Multiple JSON values
}

TEST_F(JsonParserTest, InvalidJsonBadStart) {
  assert_parse_failure(make_token_queue({}));
  make_token_queue({{"\"", json::TokenType::QUOTE},
                    {"key", json::TokenType::STRING},
                    {"\"", json::TokenType::QUOTE},
                    {":", json::TokenType::COLON}});  // Missing {
}

TEST_F(JsonParserTest, InvalidJsonParseObject) {
  assert_parse_failure(make_token_queue(
      {{".",
        json::TokenType::OBJECT_START}}));  // Token string and type mismatch
  assert_parse_failure(make_token_queue(
      {{"{", json::TokenType::OBJECT_START}}));  // Incomplete object
  assert_parse_failure(make_token_queue(
      {{"{", json::TokenType::OBJECT_START},
       {"\"", json::TokenType::QUOTE},
       {"key", json::TokenType::STRING},
       {"\"", json::TokenType::QUOTE},
       {":", json::TokenType::COLON},
       {"\"", json::TokenType::QUOTE},
       {"value", json::TokenType::STRING},
       {"\"", json::TokenType::QUOTE},
       {".", json::TokenType::OBJECT_END}}));  // Token string and type mismatch
  assert_parse_failure(make_token_queue(
      {{"{", json::TokenType::OBJECT_START},
       {"\"", json::TokenType::QUOTE},
       {"key", json::TokenType::STRING},
       {"\"", json::TokenType::QUOTE},
       {":", json::TokenType::COLON},
       {"\"", json::TokenType::QUOTE},
       {"value", json::TokenType::STRING},
       {"\"", json::TokenType::QUOTE},
       {".", json::TokenType::COMMA}}));  // Token string and type mismatch
  assert_parse_failure(
      make_token_queue({{"{", json::TokenType::OBJECT_START},
                        {"\"", json::TokenType::QUOTE},
                        {"key", json::TokenType::STRING},
                        {"\"", json::TokenType::QUOTE},
                        {":", json::TokenType::COLON},
                        {"\"", json::TokenType::QUOTE},
                        {"value", json::TokenType::STRING},
                        {"\"", json::TokenType::QUOTE},
                        {".", json::TokenType::STRING}}));  // Default
  assert_parse_failure(
      make_token_queue({{"{", json::TokenType::OBJECT_START},
                        {"\"", json::TokenType::QUOTE},
                        {"key", json::TokenType::STRING},
                        {"\"", json::TokenType::QUOTE},
                        {":", json::TokenType::COLON}}));  // Missing value
  assert_parse_failure(
      make_token_queue({{"{", json::TokenType::OBJECT_START},
                        {"\"", json::TokenType::QUOTE},
                        {"key", json::TokenType::STRING},
                        {"\"", json::TokenType::QUOTE},
                        {":", json::TokenType::COLON},
                        {"}", json::TokenType::OBJECT_END}}));  // Empty value
  assert_parse_failure(make_token_queue(
      {{"{", json::TokenType::OBJECT_START},
       {"\"", json::TokenType::QUOTE},
       {"key", json::TokenType::STRING},
       {"\"", json::TokenType::QUOTE},
       {":", json::TokenType::COLON},
       {"\"", json::TokenType::QUOTE},
       {"value", json::TokenType::STRING},
       {"\"", json::TokenType::QUOTE},
       {",", json::TokenType::COMMA},
       {"}", json::TokenType::OBJECT_END}}));  // Trailing comma
  assert_parse_failure(make_token_queue(
      {{"{", json::TokenType::OBJECT_START},
       {"\"", json::TokenType::QUOTE},
       {"key", json::TokenType::STRING},
       {"\"", json::TokenType::QUOTE},
       {":", json::TokenType::COLON},
       {"\"", json::TokenType::QUOTE},
       {"value", json::TokenType::STRING},
       {"\"", json::TokenType::QUOTE}}));  // Missing closing brace
}

TEST_F(JsonParserTest, InvalidJsonParseArray) {
  assert_parse_failure(make_token_queue(
      {{".",
        json::TokenType::ARRAY_START}}));  // Token string and type mismatch
  assert_parse_failure(make_token_queue(
      {{"[", json::TokenType::ARRAY_START}}));  // Incomplete array
  assert_parse_failure(
      make_token_queue({{"[", json::TokenType::ARRAY_START},
                        {",", json::TokenType::COMMA},
                        {"]", json::TokenType::ARRAY_END}}));  // Empty element
  assert_parse_failure(
      make_token_queue({{"[", json::TokenType::ARRAY_START},
                        {"1", json::TokenType::NUMBER},
                        {",", json::TokenType::COMMA},
                        {"]", json::TokenType::ARRAY_END}}));  // Trailing comma
  assert_parse_failure(make_token_queue(
      {{"[", json::TokenType::ARRAY_START},
       {"1", json::TokenType::NUMBER},
       {".", json::TokenType::ARRAY_END}}));  // Token string and type mismatch
  assert_parse_failure(make_token_queue(
      {{"[", json::TokenType::ARRAY_START},
       {"1", json::TokenType::NUMBER},
       {"[", json::TokenType::COMMA}}));  // Token string and type mismatch
  assert_parse_failure(
      make_token_queue({{"[", json::TokenType::ARRAY_START},
                        {"1", json::TokenType::NUMBER},
                        {"[", json::TokenType::NUMBER}}));  // Default
  assert_parse_failure(make_token_queue(
      {{"[", json::TokenType::ARRAY_START},
       {"1", json::TokenType::NUMBER}}));  // Missing closing bracket
}

TEST_F(JsonParserTest, InvalidJsonParseString) {
  assert_parse_failure(make_token_queue(
      {{"[", json::TokenType::ARRAY_START},
       {".", json::TokenType::QUOTE},
       {"]", json::TokenType::ARRAY_END}}));  // Token string and type mismatch
  assert_parse_failure(make_token_queue({{"[", json::TokenType::ARRAY_START},
                                         {"\"", json::TokenType::QUOTE},
                                         {".", json::TokenType::STRING},
                                         {".", json::TokenType::QUOTE},
                                         {"]", json::TokenType::ARRAY_END}

  }));  // Token string and type mismatch
  assert_parse_failure(make_token_queue({{"[", json::TokenType::ARRAY_START},
                                         {"\"", json::TokenType::QUOTE},
                                         {".", json::TokenType::QUOTE},
                                         {"]", json::TokenType::ARRAY_END}

  }));  // Token string and type mismatch
  assert_parse_failure(make_token_queue(
      {{"\"", json::TokenType::QUOTE},
       {"\\", json::TokenType::STRING}}));  // Incomplete escape sequence
}

TEST_F(JsonParserTest, InvalidJsonParseNumber) {
  assert_parse_failure(make_token_queue(
      {{"[", json::TokenType::ARRAY_START},
       {".", json::TokenType::NUMBER},
       {"]", json::TokenType::ARRAY_END}}));  // Just a decimal point
  assert_parse_failure(make_token_queue(
      {{"[", json::TokenType::ARRAY_START},
       {"e10", json::TokenType::NUMBER},
       {"]", json::TokenType::ARRAY_END}}));  // Just an exponent
  assert_parse_failure(make_token_queue(
      {{"[", json::TokenType::ARRAY_START},
       {"1e", json::TokenType::NUMBER},
       {"]", json::TokenType::ARRAY_END}}));  // Incomplete exponent
  assert_parse_failure(make_token_queue(
      {{"[", json::TokenType::ARRAY_START},
       {"1e+", json::TokenType::NUMBER},
       {"]", json::TokenType::ARRAY_END}}));  // Incomplete exponent with sign
  assert_parse_failure(make_token_queue(
      {{"[", json::TokenType::ARRAY_START},
       {"f", json::TokenType::NUMBER},
       {"]", json::TokenType::ARRAY_END}}));  // Incomplete exponent with sign
}

TEST_F(JsonParserTest, InvalidJsonParseBoolean) {
  assert_parse_failure(make_token_queue(
      {{"[", json::TokenType::ARRAY_START},
       {"t", json::TokenType::BOOLEAN},
       {"]", json::TokenType::ARRAY_END}}));  // Incomplete true
  assert_parse_failure(make_token_queue(
      {{"[", json::TokenType::ARRAY_START},
       {"f", json::TokenType::BOOLEAN},
       {"]", json::TokenType::ARRAY_END}}));  // Incomplete false
  assert_parse_failure(make_token_queue(
      {{"[", json::TokenType::ARRAY_START},
       {"TRUE", json::TokenType::BOOLEAN},
       {"]", json::TokenType::ARRAY_END}}));  // Incorrect capitalization
  assert_parse_failure(make_token_queue(
      {{"[", json::TokenType::ARRAY_START},
       {"FALSE", json::TokenType::BOOLEAN},
       {"]", json::TokenType::ARRAY_END}}));  // Incorrect capitalization
}

TEST_F(JsonParserTest, InvalidJsonParseNull) {
  assert_parse_failure(make_token_queue(
      {{"[", json::TokenType::ARRAY_START},
       {"n", json::TokenType::JSON_NULL},
       {"]", json::TokenType::ARRAY_END}}));  // Incomplete null
  assert_parse_failure(make_token_queue(
      {{"[", json::TokenType::ARRAY_START},
       {"NULL", json::TokenType::JSON_NULL},
       {"]", json::TokenType::ARRAY_END}}));  // Incorrect capitalization
}

TEST_F(JsonParserTest, InvalidJsonParseKeyValue) {
  assert_parse_failure(make_token_queue(
      {{"[", json::TokenType::ARRAY_START},
       {"{", json::TokenType::OBJECT_START},
       {"\"key\"", json::TokenType::STRING},
       {"]", json::TokenType::ARRAY_END}}));  // Missing colon and value
  assert_parse_failure(make_token_queue(
      {{"[", json::TokenType::ARRAY_START},
       {"{", json::TokenType::OBJECT_START},
       {"\"", json::TokenType::QUOTE},
       {"key", json::TokenType::STRING},
       {"\"", json::TokenType::QUOTE},
       {".", json::TokenType::COLON},
       {"]", json::TokenType::ARRAY_END}}));  // Token string and type mistmatch
  assert_parse_failure(make_token_queue(
      {{"[", json::TokenType::ARRAY_START},
       {"{", json::TokenType::OBJECT_START},
       {"\"", json::TokenType::QUOTE},
       {"key", json::TokenType::STRING},
       {"\"", json::TokenType::QUOTE},
       {":", json::TokenType::COLON},
       {"\"", json::TokenType::QUOTE},
       {"\"", json::TokenType::QUOTE},
       {"}", json::TokenType::OBJECT_END},
       {"]", json::TokenType::ARRAY_END}}));  // Token string and type mismatch
}
