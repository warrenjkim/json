#include "warren/internal/dsa/pair.h"

#include <string>

#include "gtest/gtest.h"

using json::utils::Pair;

class PairTest : public ::testing::Test {
 protected:
  void SetUp() override {}
};

TEST_F(PairTest, DefaultConstructor) {
  Pair<int, std::string> pair;
  EXPECT_EQ(pair.first, 0);
  EXPECT_EQ(pair.second, "");
}

TEST_F(PairTest, ParameterizedConstructor) {
  Pair<int, std::string> pair(5, "test");
  EXPECT_EQ(pair.first, 5);
  EXPECT_EQ(pair.second, "test");
}

TEST_F(PairTest, CopyConstructor) {
  Pair<int, std::string> pair1(10, "original");
  Pair<int, std::string> pair2(pair1);
  EXPECT_EQ(pair2.first, 10);
  EXPECT_EQ(pair2.second, "original");
}

TEST_F(PairTest, MoveConstructor) {
  Pair<int, std::string> pair1(15, "move_me");
  Pair<int, std::string> pair2(std::move(pair1));
  EXPECT_EQ(pair2.first, 15);
  EXPECT_EQ(pair2.second, "move_me");
}

TEST_F(PairTest, CopyAssignmentOperator) {
  Pair<int, std::string> pair1(20, "assign_me");
  Pair<int, std::string> pair2;
  pair2 = pair1;
  EXPECT_EQ(pair2.first, 20);
  EXPECT_EQ(pair2.second, "assign_me");
}

TEST_F(PairTest, MoveAssignmentOperator) {
  Pair<int, std::string> pair1(25, "move_assign_me");
  Pair<int, std::string> pair2;
  pair2 = std::move(pair1);
  EXPECT_EQ(pair2.first, 25);
  EXPECT_EQ(pair2.second, "move_assign_me");
}

TEST_F(PairTest, EqualityOperator) {
  Pair<int, std::string> pair1(30, "compare");
  Pair<int, std::string> pair2(30, "compare");
  Pair<int, std::string> pair3(35, "different");
  EXPECT_EQ(pair1, pair2);
  EXPECT_NE(pair1, pair3);
}

TEST_F(PairTest, InequalityOperator) {
  Pair<int, std::string> pair1(40, "not_equal");
  Pair<int, std::string> pair2(40, "different");
  Pair<int, std::string> pair3(40, "not_equal");
  EXPECT_NE(pair1, pair2);
  EXPECT_FALSE(pair1 != pair3);
}

TEST_F(PairTest, DifferentTypes) {
  Pair<double, char> pair(3.14, 'a');
  EXPECT_DOUBLE_EQ(pair.first, 3.14);
  EXPECT_EQ(pair.second, 'a');
}

TEST_F(PairTest, SelfAssignment) {
  Pair<int, std::string> pair(50, "self_assign");
  pair = pair;
  EXPECT_EQ(pair.first, 50);
  EXPECT_EQ(pair.second, "self_assign");

  pair = std::move(pair);
  EXPECT_EQ(pair.first, 50);
  EXPECT_EQ(pair.second, "self_assign");
}
