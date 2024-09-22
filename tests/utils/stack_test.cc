#include "utils/stack.h"

#include "gtest/gtest.h"
#include "utils/logger.h"

class StackTest : public ::testing::Test {
 protected:
  void SetUp() override {
    json::utils::init_logging(boost::log::trivial::debug);
  }
};

TEST_F(StackTest, DefaultConstructor) {
  json::utils::Stack<int> stack;
  EXPECT_TRUE(stack.empty());
  EXPECT_EQ(stack.size(), 0);
  EXPECT_FALSE(stack.top().has_value());
}

TEST_F(StackTest, CopyConstructorNonEmpty) {
  json::utils::Stack<int> stack1;
  stack1.push(10);
  stack1.push(20);
  json::utils::Stack<int> stack2(stack1);
  EXPECT_EQ(stack2.size(), 2);
  EXPECT_EQ(*stack2.top(), 20);
  EXPECT_EQ(stack1.size(), 2);
  EXPECT_EQ(*stack1.top(), 20);
}

TEST_F(StackTest, CopyConstructorEmpty) {
  json::utils::Stack<int> stack1;
  json::utils::Stack<int> stack2(stack1);
  EXPECT_TRUE(stack2.empty());
  EXPECT_EQ(stack2.size(), 0);
  EXPECT_FALSE(stack2.top().has_value());
}

TEST_F(StackTest, MoveConstructor) {
  json::utils::Stack<int> stack1;
  stack1.push(5);
  stack1.push(15);
  json::utils::Stack<int> stack2(std::move(stack1));
  EXPECT_EQ(stack2.size(), 2);
  EXPECT_EQ(*stack2.top(), 15);
  EXPECT_TRUE(stack1.empty());
  EXPECT_EQ(stack1.size(), 0);
}

TEST_F(StackTest, CopyAssignment) {
  json::utils::Stack<int> stack1;
  stack1.push(30);
  stack1.push(40);
  json::utils::Stack<int> stack2;
  stack2 = stack1;
  EXPECT_EQ(stack2.size(), 2);
  EXPECT_EQ(*stack2.top(), 40);
  EXPECT_EQ(stack1.size(), 2);
  EXPECT_EQ(*stack1.top(), 40);
}

TEST_F(StackTest, MoveAssignment) {
  json::utils::Stack<int> stack1;
  stack1.push(60);
  stack1.push(70);
  json::utils::Stack<int> stack2;
  stack2 = std::move(stack1);
  EXPECT_EQ(stack2.size(), 2);
  EXPECT_EQ(*stack2.top(), 70);
  EXPECT_TRUE(stack1.empty());
  EXPECT_EQ(stack1.size(), 0);
}

TEST_F(StackTest, PushLValue) {
  json::utils::Stack<int> stack;
  int value = 10;
  stack.push(value);
  EXPECT_EQ(stack.size(), 1);
  EXPECT_EQ(*stack.top(), 10);
}

TEST_F(StackTest, PushRValue) {
  json::utils::Stack<std::string> stack;
  std::string str = "test";
  stack.push(std::move(str));
  EXPECT_EQ(stack.size(), 1);
  EXPECT_EQ(*stack.top(), "test");
  EXPECT_TRUE(str.empty());
}

TEST_F(StackTest, PopNonEmpty) {
  json::utils::Stack<int> stack;
  stack.push(5);
  stack.push(15);
  stack.pop();
  EXPECT_EQ(stack.size(), 1);
  EXPECT_EQ(*stack.top(), 5);
}

TEST_F(StackTest, PopEmpty) {
  json::utils::Stack<int> stack;
  stack.pop();
  EXPECT_TRUE(stack.empty());
}

TEST_F(StackTest, TopNonEmpty) {
  json::utils::Stack<int> stack;
  stack.push(10);
  stack.push(20);
  EXPECT_EQ(*stack.top(), 20);
}

TEST_F(StackTest, TopEmpty) {
  json::utils::Stack<int> stack;
  EXPECT_FALSE(stack.top().has_value());
}

TEST_F(StackTest, TopConst) {
  const json::utils::Stack<int> const_ref_empty_stack;
  EXPECT_FALSE(const_ref_empty_stack.top().has_value());

  json::utils::Stack<int> non_empty_stack;
  non_empty_stack.push(5);
  non_empty_stack.push(10);
  const json::utils::Stack<int>& const_ref_stack = non_empty_stack;
  auto top_value = const_ref_stack.top();
  EXPECT_TRUE(top_value.has_value());
  EXPECT_EQ(*top_value, 10);
}

TEST_F(StackTest, Clear) {
  json::utils::Stack<int> stack;
  stack.push(1);
  stack.push(2);
  stack.push(3);
  stack.clear();
  EXPECT_TRUE(stack.empty());
  EXPECT_EQ(stack.size(), 0);
  EXPECT_FALSE(stack.top().has_value());
}

TEST_F(StackTest, SizeAndEmpty) {
  json::utils::Stack<int> stack;
  EXPECT_TRUE(stack.empty());
  EXPECT_EQ(stack.size(), 0);
  stack.push(1);
  EXPECT_FALSE(stack.empty());
  EXPECT_EQ(stack.size(), 1);
  stack.push(2);
  EXPECT_EQ(stack.size(), 2);
  stack.pop();
  EXPECT_EQ(stack.size(), 1);
  stack.pop();
  EXPECT_TRUE(stack.empty());
  EXPECT_EQ(stack.size(), 0);
}

TEST_F(StackTest, EmptyStackOperations) {
  json::utils::Stack<int> stack;

  EXPECT_TRUE(stack.empty());
  EXPECT_EQ(stack.size(), 0);
  EXPECT_FALSE(stack.top().has_value());

  stack.pop();
  EXPECT_TRUE(stack.empty());
  EXPECT_EQ(stack.size(), 0);

  stack.push(5);
  EXPECT_FALSE(stack.empty());
  EXPECT_EQ(stack.size(), 1);
  EXPECT_EQ(*stack.top(), 5);

  stack.clear();
  EXPECT_TRUE(stack.empty());
  EXPECT_EQ(stack.size(), 0);
  EXPECT_FALSE(stack.top().has_value());
}
