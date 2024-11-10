#include "visitors/get_visitor.h"

#include <gtest/gtest.h>

#include <boost/log/trivial.hpp>
#include <stdexcept>

#include "json/exception.h"
#include "json/value.h"
#include "nodes/array.h"
#include "nodes/boolean.h"
#include "nodes/node.h"
#include "nodes/null.h"
#include "nodes/number.h"
#include "nodes/object.h"
#include "nodes/string.h"
#include "utils/logger.h"

class GetVisitorTest : public ::testing::Test {
 protected:
  void SetUp() override {
    json::utils::init_logging(boost::log::trivial::debug);
    root_ = new json::Object();

    name_ = new json::Object();
    name_->add("first", new json::String("john"));
    name_->add("last", new json::String("doe"));
    root_->add("name", name_);

    hobbies_ = new json::Array();
    hobbies_->add(new json::String("reading"));
    hobbies_->add(new json::String("cycling"));
    root_->add("hobbies", hobbies_);

    root_->add("age", new json::Number(30));
    root_->add("balance", new json::Number(99.99));
    root_->add("is_active", new json::Boolean(true));
    root_->add("nullable", new json::Null());

    numbers_ = new json::Array();
    numbers_->add(new json::Number(-1));
    numbers_->add(new json::Number(0));
    numbers_->add(new json::Number(3.14));
    root_->add("numbers", numbers_);

    null_ = new json::Null();
    root_->add("null_node", null_);
  }

  void TearDown() override { delete root_; }

  void assert_get(json::utils::Queue<std::string>& keys,
                  const json::Value& expected) {
    json::Value result;
    json::visitors::GetVisitor visitor(keys);
    root_->accept(visitor);
    ASSERT_EQ(result, expected);
  }

  template <typename ExceptionType>
  void assert_not_get(json::utils::Queue<std::string>& keys) {
    json::Value result;
    json::visitors::GetVisitor visitor(keys);
    ASSERT_THROW(root_->accept(visitor), ExceptionType);
  }

  json::Object* root_;
  json::Object* name_;
  json::Array* hobbies_;
  json::Array* numbers_;
  json::Null* null_;
};

TEST_F(GetVisitorTest, GetNull) {
  // arrange
  json::utils::Queue<std::string> keys;
  keys.emplace("null_node");

  // act
  json::visitors::GetVisitor visitor(keys);
  root_->accept(visitor);

  // assert
  ASSERT_EQ(*visitor.result(), *(new json::Null()));
}

TEST_F(GetVisitorTest, GetInt) {
  // arrange
  json::utils::Queue<std::string> keys;
  keys.emplace("age");

  // act
  json::visitors::GetVisitor visitor(keys);
  root_->accept(visitor);

  // assert
  ASSERT_EQ(*visitor.result(), *(new json::Number(30)));
}

TEST_F(GetVisitorTest, GetDouble) {
  // arrange
  json::utils::Queue<std::string> keys;
  keys.emplace("balance");

  // act
  json::visitors::GetVisitor visitor(keys);
  root_->accept(visitor);

  // assert
  ASSERT_EQ(*visitor.result(), *(new json::Number(99.99)));
}

TEST_F(GetVisitorTest, GetBoolean) {
  // arrange
  json::utils::Queue<std::string> keys;
  keys.emplace("is_active");

  // act
  json::visitors::GetVisitor visitor(keys);
  root_->accept(visitor);

  // assert
  ASSERT_EQ(*visitor.result(), *(new json::Boolean(true)));
}

TEST_F(GetVisitorTest, GetNestedString) {
  // arrange
  json::utils::Queue<std::string> keys;
  keys.emplace("name");
  keys.emplace("first");

  // act
  json::visitors::GetVisitor visitor(keys);
  root_->accept(visitor);

  // assert
  ASSERT_EQ(*visitor.result(), *(new json::String("john")));
}

TEST_F(GetVisitorTest, GetArray) {
  // arrange
  json::utils::Queue<std::string> keys;
  keys.emplace("numbers");

  // act
  json::visitors::GetVisitor visitor(keys);
  root_->accept(visitor);

  // assert
  ASSERT_EQ(*visitor.result(), *numbers_);
}

TEST_F(GetVisitorTest, GetObject) {
  // arrange
  json::utils::Queue<std::string> keys;
  keys.emplace("name");

  // act
  json::visitors::GetVisitor visitor(keys);
  root_->accept(visitor);

  // assert
  ASSERT_EQ(*visitor.result(), *name_);
}

TEST_F(GetVisitorTest, GetArrayNumberAsDouble) {
  // arrange
  json::utils::Queue<std::string> keys;
  keys.emplace("numbers");
  keys.emplace("2");

  // act
  json::visitors::GetVisitor visitor(keys);
  root_->accept(visitor);

  // assert
  ASSERT_EQ(*visitor.result(), *(new json::Number(3.14)));
}

TEST_F(GetVisitorTest, InvalidPath) {
  // arrange
  json::utils::Queue<std::string> keys;
  keys.emplace("invalid");
  keys.emplace("path");

  // act + assert
  assert_not_get<json::NotFoundException>(keys);
}

TEST_F(GetVisitorTest, OutOfBoundsArray) {
  // arrange
  json::utils::Queue<std::string> keys;
  keys.emplace("hobbies");
  keys.emplace("99");

  // act + assert
  assert_not_get<std::out_of_range>(keys);
}

TEST_F(GetVisitorTest, InvalidArgument) {
  // arrange
  json::utils::Queue<std::string> keys;
  keys.emplace("hobbies");
  keys.emplace("invalid");

  // act + assert
  assert_not_get<std::invalid_argument>(keys);
}

TEST_F(GetVisitorTest, BadAccessOnNumber) {
  // arrange
  json::utils::Queue<std::string> keys;
  keys.emplace("age");
  keys.emplace("something");

  // act + assert
  assert_not_get<json::BadAccessException>(keys);
}

TEST_F(GetVisitorTest, BadAccessOnBoolean) {
  // arrange
  json::utils::Queue<std::string> keys;
  keys.emplace("is_active");
  keys.emplace("something");

  // act + assert
  assert_not_get<json::BadAccessException>(keys);
}

TEST_F(GetVisitorTest, BadAccessOnNull) {
  // arrange
  json::utils::Queue<std::string> keys;
  keys.emplace("nullable");
  keys.emplace("something");

  // act + assert
  assert_not_get<json::BadAccessException>(keys);
}

TEST_F(GetVisitorTest, BadAccessOnString) {
  // arrange
  json::utils::Queue<std::string> keys;
  keys.emplace("hobbies");
  keys.emplace("0");
  keys.emplace("something");

  // act + assert
  assert_not_get<json::BadAccessException>(keys);
}
