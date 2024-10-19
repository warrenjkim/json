#include "nodes/visitors/cmp_visitor.h"

#include <gtest/gtest.h>

#include <boost/log/trivial.hpp>

#include "nodes/array_node.h"
#include "nodes/boolean_node.h"
#include "nodes/node.h"
#include "nodes/null_node.h"
#include "nodes/number_node.h"
#include "nodes/object_node.h"
#include "nodes/string_node.h"
#include "utils/logger.h"

class CmpVisitorTest : public ::testing::Test {
 protected:
  void SetUp() override {
    json::utils::init_logging(boost::log::trivial::debug);
  }

  void assert_equal(json::Node* ast_1, json::Node* ast_2) {
    json::nodes::visitors::CmpVisitor visitor(ast_2);
    ast_1->accept(visitor);
    ASSERT_TRUE(visitor.result()) << "ASTs are not equal";
    delete ast_1;
    delete ast_2;
  }

  void assert_not_equal(json::Node* ast_1, json::Node* ast_2) {
    json::nodes::visitors::CmpVisitor visitor(ast_2);
    ast_1->accept(visitor);
    ASSERT_FALSE(visitor.result()) << "ASTs are unexpectedly equal";
    delete ast_1;
    delete ast_2;
  }
};

TEST_F(CmpVisitorTest, EmptyObject) {
  assert_equal(new json::ObjectNode(), new json::ObjectNode());
}

TEST_F(CmpVisitorTest, SimpleObject) {
  json::ObjectNode* obj_1 = new json::ObjectNode();
  obj_1->add("key", new json::StringNode("value"));

  json::ObjectNode* obj_2 = new json::ObjectNode();
  obj_2->add("key", new json::StringNode("value"));

  assert_equal(obj_1, obj_2);
}

TEST_F(CmpVisitorTest, DifferentObjects) {
  json::ObjectNode* obj_1 = new json::ObjectNode();
  obj_1->add("key_1", new json::StringNode("value_1"));

  json::ObjectNode* obj_2 = new json::ObjectNode();
  obj_2->add("key_2", new json::StringNode("value_2"));

  assert_not_equal(obj_1, obj_2);
}

TEST_F(CmpVisitorTest, EmptyArray) {
  assert_equal(new json::ArrayNode(), new json::ArrayNode());
}

TEST_F(CmpVisitorTest, SimpleArray) {
  json::ArrayNode* arr_1 = new json::ArrayNode();
  arr_1->add(new json::NumberNode(1));
  arr_1->add(new json::NumberNode(2));
  arr_1->add(new json::NumberNode(3));

  json::ArrayNode* arr_2 = new json::ArrayNode();
  arr_2->add(new json::NumberNode(1));
  arr_2->add(new json::NumberNode(2));
  arr_2->add(new json::NumberNode(3));

  assert_equal(arr_1, arr_2);
}

TEST_F(CmpVisitorTest, DifferentArrays) {
  json::ArrayNode* arr_1 = new json::ArrayNode();
  arr_1->add(new json::NumberNode(1));
  arr_1->add(new json::NumberNode(2));

  json::ArrayNode* arr_2 = new json::ArrayNode();
  arr_2->add(new json::NumberNode(1));
  arr_2->add(new json::NumberNode(3));

  assert_not_equal(arr_1, arr_2);
}

TEST_F(CmpVisitorTest, LogicalValues) {
  json::ArrayNode* arr_1 = new json::ArrayNode();
  arr_1->add(new json::BooleanNode(true));
  arr_1->add(new json::BooleanNode(false));
  arr_1->add(new json::NullNode());

  json::ArrayNode* arr_2 = new json::ArrayNode();
  arr_2->add(new json::BooleanNode(true));
  arr_2->add(new json::BooleanNode(false));
  arr_2->add(new json::NullNode());

  assert_equal(arr_1, arr_2);
}

TEST_F(CmpVisitorTest, DifferentLogicalValues) {
  json::ArrayNode* arr_1 = new json::ArrayNode();
  arr_1->add(new json::BooleanNode(true));
  arr_1->add(new json::NullNode());

  json::ArrayNode* arr_2 = new json::ArrayNode();
  arr_2->add(new json::BooleanNode(false));
  arr_2->add(new json::NullNode());

  assert_not_equal(arr_1, arr_2);
}

TEST_F(CmpVisitorTest, Numbers) {
  json::ArrayNode* arr_1 = new json::ArrayNode();
  arr_1->add(new json::NumberNode(0));
  arr_1->add(new json::NumberNode(42));
  arr_1->add(new json::NumberNode(-42));
  arr_1->add(new json::NumberNode(3.14));

  json::ArrayNode* arr_2 = new json::ArrayNode();
  arr_2->add(new json::NumberNode(0));
  arr_2->add(new json::NumberNode(42));
  arr_2->add(new json::NumberNode(-42));
  arr_2->add(new json::NumberNode(3.14));

  assert_equal(arr_1, arr_2);
}

TEST_F(CmpVisitorTest, DifferentNumbers) {
  json::ArrayNode* arr_1 = new json::ArrayNode();
  arr_1->add(new json::NumberNode(1));
  arr_1->add(new json::NumberNode(2));

  json::ArrayNode* arr_2 = new json::ArrayNode();
  arr_2->add(new json::NumberNode(1));
  arr_2->add(new json::NumberNode(2.1));

  assert_not_equal(arr_1, arr_2);
}

TEST_F(CmpVisitorTest, Strings) {
  json::ArrayNode* arr_1 = new json::ArrayNode();
  arr_1->add(new json::StringNode("hello"));
  arr_1->add(new json::StringNode("world"));

  json::ArrayNode* arr_2 = new json::ArrayNode();
  arr_2->add(new json::StringNode("hello"));
  arr_2->add(new json::StringNode("world"));

  assert_equal(arr_1, arr_2);
}

TEST_F(CmpVisitorTest, DifferentStrings) {
  json::ArrayNode* arr_1 = new json::ArrayNode();
  arr_1->add(new json::StringNode("hello"));

  json::ArrayNode* arr_2 = new json::ArrayNode();
  arr_2->add(new json::StringNode("world"));

  assert_not_equal(arr_1, arr_2);
}

TEST_F(CmpVisitorTest, ComplexStructure) {
  json::ObjectNode* obj_1 = new json::ObjectNode();
  json::ObjectNode* inner_obj_1 = new json::ObjectNode();
  inner_obj_1->add("first", new json::StringNode("john"));
  inner_obj_1->add("last", new json::StringNode("doe"));
  obj_1->add("name", inner_obj_1);
  obj_1->add("age", new json::NumberNode(30));
  obj_1->add("is_student", new json::BooleanNode(false));

  json::ArrayNode* hobbies_1 = new json::ArrayNode();
  hobbies_1->add(new json::StringNode("reading"));
  hobbies_1->add(new json::StringNode("cycling"));

  obj_1->add("hobbies", hobbies_1);
  obj_1->add("address", new json::NullNode());

  json::ObjectNode* obj_2 = new json::ObjectNode();
  json::ObjectNode* inner_obj_2 = new json::ObjectNode();
  inner_obj_2->add("first", new json::StringNode("john"));
  inner_obj_2->add("last", new json::StringNode("doe"));
  obj_2->add("name", inner_obj_2);
  obj_2->add("age", new json::NumberNode(30));
  obj_2->add("is_student", new json::BooleanNode(false));

  json::ArrayNode* hobbies_2 = new json::ArrayNode();
  hobbies_2->add(new json::StringNode("reading"));
  hobbies_2->add(new json::StringNode("cycling"));

  obj_2->add("hobbies", hobbies_2);
  obj_2->add("address", new json::NullNode());

  assert_equal(obj_1, obj_2);
}

TEST_F(CmpVisitorTest, DifferentComplexStructures) {
  json::ObjectNode* obj_1 = new json::ObjectNode();
  obj_1->add("name", new json::StringNode("john"));
  obj_1->add("age", new json::NumberNode(30));

  json::ObjectNode* obj_2 = new json::ObjectNode();
  obj_2->add("name", new json::StringNode("john"));
  obj_2->add("age", new json::NumberNode(31));

  assert_not_equal(obj_1, obj_2);
}
