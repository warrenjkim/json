#include "visitors/get_visitor.h"

#include <gtest/gtest.h>

#include <boost/log/trivial.hpp>

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
  }

  void TearDown() override { delete root_; }

  void assert_get(const json::utils::Queue<std::string>& keys,
                  json::Node* expected) {
    json::visitors::GetVisitor visitor(root_, keys);
    json::Node* result = root_->accept(visitor);
    if (!expected) {
      ASSERT_EQ(result, nullptr);
    } else {
      ASSERT_TRUE(result);
      ASSERT_EQ(*expected, *result);
    }
  }

  json::Object* root_;
  json::Object* name_;
  json::Array* hobbies_;
  json::Array* numbers_;
};

TEST_F(GetVisitorTest, GetBasicNodes) {
  json::utils::Queue<std::string> keys;

  assert_get(keys, root_);

  keys.emplace("age");
  assert_get(keys, new json::Number(30));
  keys = json::utils::Queue<std::string>();
  keys.emplace("balance");
  assert_get(keys, new json::Number(99.99));

  keys = json::utils::Queue<std::string>();
  keys.emplace("is_active");
  assert_get(keys, new json::Boolean(true));

  keys = json::utils::Queue<std::string>();
  keys.emplace("nullable");
  assert_get(keys, new json::Null());
}

TEST_F(GetVisitorTest, ConstGetBasicNodes) {
  const json::Object* const_root = root_;
  json::utils::Queue<std::string> keys;

  json::visitors::GetVisitor const_visitor_root(const_root, keys);
  const json::Node* result = const_root->accept(const_visitor_root);
  ASSERT_TRUE(result);
  ASSERT_EQ(*result, *root_);

  keys.emplace("age");
  json::visitors::GetVisitor const_visitor_age(const_root, keys);
  result = const_root->accept(const_visitor_age);
  ASSERT_TRUE(result);
  ASSERT_EQ(*result, *new json::Number(30));

  keys = json::utils::Queue<std::string>();
  keys.emplace("balance");
  json::visitors::GetVisitor const_visitor_balance(const_root, keys);
  result = const_root->accept(const_visitor_balance);
  ASSERT_TRUE(result);
  ASSERT_EQ(*result, *new json::Number(99.99));

  keys = json::utils::Queue<std::string>();
  keys.emplace("is_active");
  json::visitors::GetVisitor const_visitor_bool(const_root, keys);
  result = const_root->accept(const_visitor_bool);
  ASSERT_TRUE(result);
  ASSERT_EQ(*result, *new json::Boolean(true));

  keys = json::utils::Queue<std::string>();
  keys.emplace("nullable");
  json::visitors::GetVisitor const_visitor_null(const_root, keys);
  result = const_root->accept(const_visitor_null);
  ASSERT_TRUE(result);
  ASSERT_EQ(*result, *new json::Null());
}

TEST_F(GetVisitorTest, GetNestedValues) {
  json::utils::Queue<std::string> keys;

  keys.emplace("name");
  assert_get(keys, name_);

  keys.emplace("first");
  assert_get(keys, new json::String("john"));

  keys = json::utils::Queue<std::string>();
  keys.emplace("hobbies");
  assert_get(keys, hobbies_);

  keys.emplace("0");
  assert_get(keys, new json::String("reading"));
}

TEST_F(GetVisitorTest, GetNumberArray) {
  json::utils::Queue<std::string> keys;
  keys.emplace("numbers");
  assert_get(keys, numbers_);

  keys.emplace("0");
  assert_get(keys, new json::Number(-1));

  keys = json::utils::Queue<std::string>();
  keys.emplace("numbers");
  keys.emplace("1");
  assert_get(keys, new json::Number(0));

  keys = json::utils::Queue<std::string>();
  keys.emplace("numbers");
  keys.emplace("2");
  assert_get(keys, new json::Number(3.14));
}

TEST_F(GetVisitorTest, InvalidPaths) {
  json::utils::Queue<std::string> keys;

  keys.emplace("invalid");
  keys.emplace("path");
  json::visitors::GetVisitor visitor(root_, keys);
  ASSERT_EQ(root_->accept(visitor), nullptr);

  keys = json::utils::Queue<std::string>();
  keys.emplace("hobbies");
  keys.emplace("99");
  visitor = json::visitors::GetVisitor(root_, keys);
  ASSERT_EQ(root_->accept(visitor), nullptr);

  keys = json::utils::Queue<std::string>();
  keys.emplace("numbers");
  keys.emplace("abc");
  visitor = json::visitors::GetVisitor(root_, keys);
  ASSERT_EQ(root_->accept(visitor), nullptr);
}
