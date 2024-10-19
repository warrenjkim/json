#include "visitors/cmp_visitor.h"

#include <gtest/gtest.h>

#include <boost/log/trivial.hpp>

#include "types/array.h"
#include "types/boolean.h"
#include "types/type.h"
#include "types/null.h"
#include "types/number.h"
#include "types/object.h"
#include "types/string.h"
#include "utils/logger.h"

class CmpVisitorTest : public ::testing::Test {
 protected:
  void SetUp() override {
    json::utils::init_logging(boost::log::trivial::debug);
  }

  void assert_equal(json::Type* ast_1, json::Type* ast_2) {
    json::visitors::CmpVisitor visitor(ast_2);
    ast_1->accept(visitor);
    ASSERT_TRUE(visitor.result()) << "ASTs are not equal";
    delete ast_1;
    delete ast_2;
  }

  void assert_not_equal(json::Type* ast_1, json::Type* ast_2) {
    json::visitors::CmpVisitor visitor(ast_2);
    ast_1->accept(visitor);
    ASSERT_FALSE(visitor.result()) << "ASTs are unexpectedly equal";
    delete ast_1;
    delete ast_2;
  }
};

TEST_F(CmpVisitorTest, EmptyObject) {
  assert_equal(new json::Object(), new json::Object());
}

TEST_F(CmpVisitorTest, SimpleObject) {
  json::Object* obj_1 = new json::Object();
  obj_1->add("key", new json::String("value"));

  json::Object* obj_2 = new json::Object();
  obj_2->add("key", new json::String("value"));

  assert_equal(obj_1, obj_2);
}

TEST_F(CmpVisitorTest, DifferentObjects) {
  json::Object* obj_1 = new json::Object();
  obj_1->add("key_1", new json::String("value_1"));

  json::Object* obj_2 = new json::Object();
  obj_2->add("key_2", new json::String("value_2"));

  assert_not_equal(obj_1, obj_2);
}

TEST_F(CmpVisitorTest, EmptyArray) {
  assert_equal(new json::Array(), new json::Array());
}

TEST_F(CmpVisitorTest, SimpleArray) {
  json::Array* arr_1 = new json::Array();
  arr_1->add(new json::Number(1));
  arr_1->add(new json::Number(2));
  arr_1->add(new json::Number(3));

  json::Array* arr_2 = new json::Array();
  arr_2->add(new json::Number(1));
  arr_2->add(new json::Number(2));
  arr_2->add(new json::Number(3));

  assert_equal(arr_1, arr_2);
}

TEST_F(CmpVisitorTest, DifferentArrays) {
  json::Array* arr_1 = new json::Array();
  arr_1->add(new json::Number(1));
  arr_1->add(new json::Number(2));

  json::Array* arr_2 = new json::Array();
  arr_2->add(new json::Number(1));
  arr_2->add(new json::Number(3));

  assert_not_equal(arr_1, arr_2);
}

TEST_F(CmpVisitorTest, LogicalValues) {
  json::Array* arr_1 = new json::Array();
  arr_1->add(new json::Boolean(true));
  arr_1->add(new json::Boolean(false));
  arr_1->add(new json::Null());

  json::Array* arr_2 = new json::Array();
  arr_2->add(new json::Boolean(true));
  arr_2->add(new json::Boolean(false));
  arr_2->add(new json::Null());

  assert_equal(arr_1, arr_2);
}

TEST_F(CmpVisitorTest, DifferentLogicalValues) {
  json::Array* arr_1 = new json::Array();
  arr_1->add(new json::Boolean(true));
  arr_1->add(new json::Null());

  json::Array* arr_2 = new json::Array();
  arr_2->add(new json::Boolean(false));
  arr_2->add(new json::Null());

  assert_not_equal(arr_1, arr_2);
}

TEST_F(CmpVisitorTest, Numbers) {
  json::Array* arr_1 = new json::Array();
  arr_1->add(new json::Number(0));
  arr_1->add(new json::Number(42));
  arr_1->add(new json::Number(-42));
  arr_1->add(new json::Number(3.14));

  json::Array* arr_2 = new json::Array();
  arr_2->add(new json::Number(0));
  arr_2->add(new json::Number(42));
  arr_2->add(new json::Number(-42));
  arr_2->add(new json::Number(3.14));

  assert_equal(arr_1, arr_2);
}

TEST_F(CmpVisitorTest, DifferentNumbers) {
  json::Array* arr_1 = new json::Array();
  arr_1->add(new json::Number(1));
  arr_1->add(new json::Number(2));

  json::Array* arr_2 = new json::Array();
  arr_2->add(new json::Number(1));
  arr_2->add(new json::Number(2.1));

  assert_not_equal(arr_1, arr_2);
}

TEST_F(CmpVisitorTest, Strings) {
  json::Array* arr_1 = new json::Array();
  arr_1->add(new json::String("hello"));
  arr_1->add(new json::String("world"));

  json::Array* arr_2 = new json::Array();
  arr_2->add(new json::String("hello"));
  arr_2->add(new json::String("world"));

  assert_equal(arr_1, arr_2);
}

TEST_F(CmpVisitorTest, DifferentStrings) {
  json::Array* arr_1 = new json::Array();
  arr_1->add(new json::String("hello"));

  json::Array* arr_2 = new json::Array();
  arr_2->add(new json::String("world"));

  assert_not_equal(arr_1, arr_2);
}

TEST_F(CmpVisitorTest, ComplexStructure) {
  json::Object* obj_1 = new json::Object();
  json::Object* inner_obj_1 = new json::Object();
  inner_obj_1->add("first", new json::String("john"));
  inner_obj_1->add("last", new json::String("doe"));
  obj_1->add("name", inner_obj_1);
  obj_1->add("age", new json::Number(30));
  obj_1->add("is_student", new json::Boolean(false));

  json::Array* hobbies_1 = new json::Array();
  hobbies_1->add(new json::String("reading"));
  hobbies_1->add(new json::String("cycling"));

  obj_1->add("hobbies", hobbies_1);
  obj_1->add("address", new json::Null());

  json::Object* obj_2 = new json::Object();
  json::Object* inner_obj_2 = new json::Object();
  inner_obj_2->add("first", new json::String("john"));
  inner_obj_2->add("last", new json::String("doe"));
  obj_2->add("name", inner_obj_2);
  obj_2->add("age", new json::Number(30));
  obj_2->add("is_student", new json::Boolean(false));

  json::Array* hobbies_2 = new json::Array();
  hobbies_2->add(new json::String("reading"));
  hobbies_2->add(new json::String("cycling"));

  obj_2->add("hobbies", hobbies_2);
  obj_2->add("address", new json::Null());

  assert_equal(obj_1, obj_2);
}

TEST_F(CmpVisitorTest, DifferentComplexStructures) {
  json::Object* obj_1 = new json::Object();
  obj_1->add("name", new json::String("john"));
  obj_1->add("age", new json::Number(30));

  json::Object* obj_2 = new json::Object();
  obj_2->add("name", new json::String("john"));
  obj_2->add("age", new json::Number(31));

  assert_not_equal(obj_1, obj_2);
}
