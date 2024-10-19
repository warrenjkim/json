#include "utils/queue.h"

#include "gtest/gtest.h"
#include "utils/logger.h"

class QueueTest : public ::testing::Test {
 protected:
  void SetUp() override {
    json::utils::init_logging(boost::log::trivial::debug);
  }
};

TEST_F(QueueTest, DefaultConstructor) {
  json::utils::Queue<int> queue;
  EXPECT_TRUE(queue.empty());
  EXPECT_EQ(queue.size(), 0);
  EXPECT_FALSE(queue.front().has_value());
}

TEST_F(QueueTest, CopyConstructorNonEmpty) {
  json::utils::Queue<int> queue1;
  queue1.enqueue(10);
  queue1.enqueue(20);
  json::utils::Queue<int> queue2(queue1);
  EXPECT_EQ(queue2.size(), 2);
  EXPECT_EQ(*queue2.front(), 10);
  EXPECT_EQ(queue1.size(), 2);
  EXPECT_EQ(*queue1.front(), 10);
}

TEST_F(QueueTest, CopyConstructorEmpty) {
  json::utils::Queue<int> queue1;
  json::utils::Queue<int> queue2(queue1);
  EXPECT_TRUE(queue2.empty());
  EXPECT_EQ(queue2.size(), 0);
  EXPECT_FALSE(queue2.front().has_value());
}

TEST_F(QueueTest, MoveConstructor) {
  json::utils::Queue<int> queue1;
  queue1.enqueue(5);
  queue1.enqueue(15);
  json::utils::Queue<int> queue2(std::move(queue1));
  EXPECT_EQ(queue2.size(), 2);
  EXPECT_EQ(*queue2.front(), 5);
  EXPECT_TRUE(queue1.empty());
  EXPECT_EQ(queue1.size(), 0);
}

TEST_F(QueueTest, CopyAssignment) {
  json::utils::Queue<int> queue1;
  queue1.enqueue(30);
  queue1.enqueue(40);
  json::utils::Queue<int> queue2;
  queue2 = queue1;
  EXPECT_EQ(queue2.size(), 2);
  EXPECT_EQ(*queue2.front(), 30);
  EXPECT_EQ(queue1.size(), 2);
  EXPECT_EQ(*queue1.front(), 30);
}

TEST_F(QueueTest, MoveAssignment) {
  json::utils::Queue<int> queue1;
  queue1.enqueue(60);
  queue1.enqueue(70);
  json::utils::Queue<int> queue2;
  queue2 = std::move(queue1);
  EXPECT_EQ(queue2.size(), 2);
  EXPECT_EQ(*queue2.front(), 60);
  EXPECT_TRUE(queue1.empty());
  EXPECT_EQ(queue1.size(), 0);
}

TEST_F(QueueTest, EnqueueLValue) {
  json::utils::Queue<int> queue;
  int value_1 = 10;
  int value_2 = 20;
  queue.enqueue(value_1);
  queue.enqueue(value_2);
  EXPECT_EQ(queue.size(), 2);
  EXPECT_EQ(*queue.front(), value_1);
}

TEST_F(QueueTest, EnqueueRValue) {
  json::utils::Queue<std::string> queue;
  std::string str_1 = "test1";
  std::string str_2 = "test2";
  queue.enqueue(std::move(str_1));
  queue.enqueue(std::move(str_2));
  EXPECT_EQ(queue.size(), 2);
  EXPECT_EQ(*queue.front(), "test1");
  EXPECT_TRUE(str_1.empty());
  EXPECT_TRUE(str_2.empty());
}

TEST_F(QueueTest, Emplace) {
  json::utils::Queue<std::string> queue;
  queue.emplace("test1");
  queue.emplace("test2");
  EXPECT_EQ(queue.size(), 2);
  EXPECT_EQ(*queue.front(), "test1");
}

TEST_F(QueueTest, DequeueNonEmpty) {
  json::utils::Queue<int> queue;
  queue.enqueue(5);
  queue.enqueue(15);
  queue.dequeue();
  EXPECT_EQ(queue.size(), 1);
  EXPECT_EQ(*queue.front(), 15);
}

TEST_F(QueueTest, DequeueEmpty) {
  json::utils::Queue<int> queue;
  queue.dequeue();
  EXPECT_TRUE(queue.empty());
}

TEST_F(QueueTest, FrontNonEmpty) {
  json::utils::Queue<int> queue;
  queue.enqueue(10);
  queue.enqueue(20);
  queue.enqueue(30);
  queue.enqueue(40);
  EXPECT_EQ(*queue.front(), 10);
}

TEST_F(QueueTest, FrontEmpty) {
  json::utils::Queue<int> queue;
  EXPECT_FALSE(queue.front().has_value());
}

TEST_F(QueueTest, FrontConst) {
  const json::utils::Queue<int> const_ref_empty_queue;
  EXPECT_FALSE(const_ref_empty_queue.front().has_value());

  json::utils::Queue<int> non_empty_queue;
  non_empty_queue.enqueue(5);
  non_empty_queue.enqueue(10);
  const json::utils::Queue<int>& const_ref_queue = non_empty_queue;
  auto front_value = const_ref_queue.front();
  EXPECT_TRUE(front_value.has_value());
  EXPECT_EQ(*front_value, 5);
}

TEST_F(QueueTest, Clear) {
  json::utils::Queue<int> queue;
  queue.enqueue(1);
  queue.enqueue(2);
  queue.enqueue(3);
  queue.clear();
  EXPECT_TRUE(queue.empty());
  EXPECT_EQ(queue.size(), 0);
  EXPECT_FALSE(queue.front().has_value());
}

TEST_F(QueueTest, SizeAndEmpty) {
  json::utils::Queue<int> queue;
  EXPECT_TRUE(queue.empty());
  EXPECT_EQ(queue.size(), 0);
  queue.enqueue(1);
  EXPECT_FALSE(queue.empty());
  EXPECT_EQ(queue.size(), 1);
  EXPECT_EQ(queue.front(), 1);
  queue.enqueue(2);
  EXPECT_EQ(queue.size(), 2);
  EXPECT_EQ(queue.front(), 1);
  queue.dequeue();
  EXPECT_EQ(queue.size(), 1);
  EXPECT_EQ(queue.front(), 2);
  queue.dequeue();
  EXPECT_TRUE(queue.empty());
  EXPECT_EQ(queue.size(), 0);
}

TEST_F(QueueTest, EmptyQueueOperations) {
  json::utils::Queue<int> queue;

  EXPECT_TRUE(queue.empty());
  EXPECT_EQ(queue.size(), 0);
  EXPECT_FALSE(queue.front().has_value());

  queue.dequeue();
  EXPECT_TRUE(queue.empty());
  EXPECT_EQ(queue.size(), 0);

  queue.enqueue(5);
  EXPECT_FALSE(queue.empty());
  EXPECT_EQ(queue.size(), 1);
  EXPECT_EQ(*queue.front(), 5);

  queue.clear();
  EXPECT_TRUE(queue.empty());
  EXPECT_EQ(queue.size(), 0);
  EXPECT_FALSE(queue.front().has_value());
}
