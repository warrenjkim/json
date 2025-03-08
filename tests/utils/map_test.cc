#include "utils/map.h"

#include <gtest/gtest.h>

#include <string>

class MapTest : public ::testing::Test {
 protected:
  json::utils::Map<std::string, int> map_;
};

TEST_F(MapTest, CopyConstructor) {
  // arrange
  map_.insert("key", 1);

  // act
  json::utils::Map<std::string, int> copy(map_);

  // assert
  ASSERT_EQ(copy.size(), 1);
  ASSERT_EQ(copy.at("key"), 1);
}

TEST_F(MapTest, MoveConstructor) {
  // arrange
  map_.insert("key", 1);

  // act
  json::utils::Map<std::string, int> moved(std::move(map_));

  // assert
  ASSERT_TRUE(map_.empty());
  ASSERT_EQ(moved.size(), 1);
  ASSERT_EQ(moved.at("key"), 1);
}

TEST_F(MapTest, Equality) {
  // arrange
  map_.insert("a", 1);
  map_.insert("b", 2);

  json::utils::Map<std::string, int> other;
  other.insert("a", 1);
  other.insert("b", 2);

  // act + assert
  ASSERT_TRUE(map_ == other);
  ASSERT_FALSE(map_ != other);
}

TEST_F(MapTest, SubscriptOperator) {
  // arrange + act
  map_["key"] = 1;

  // assert
  ASSERT_EQ(map_.size(), 1);
  ASSERT_EQ(map_["key"], 1);

  // arrange + act
  map_["key"] = 2;

  // assert
  ASSERT_EQ(map_.size(), 1);
  ASSERT_EQ(map_["key"], 2);

  // arrange + act + assert
  ASSERT_EQ(map_["new"], 0);
}

TEST_F(MapTest, At) {
  // arrange
  map_.insert("key", 1);

  // act
  std::optional<int> val = map_.at("key");

  // assert
  ASSERT_TRUE(val.has_value());
  ASSERT_EQ(*val, 1);

  ASSERT_FALSE(map_.at("nonexistent").has_value());
}

TEST_F(MapTest, Contains) {
  // arrange
  map_.insert("key", 1);

  // act + assert
  ASSERT_TRUE(map_.contains("key"));
  ASSERT_FALSE(map_.contains("nonexistent"));
}

TEST_F(MapTest, Insert) {
  // arrange + act
  map_.insert("key", 1);

  // assert
  ASSERT_TRUE(map_.contains("key"));
  ASSERT_EQ(map_.at("key"), 1);

  // arrange + act
  map_.insert("key", 2);

  // assert
  ASSERT_EQ(map_.at("key"), 2);
}

TEST_F(MapTest, Remove) {
  // arrange
  map_.insert("key", 1);

  // act
  map_.erase("key");

  // assert
  ASSERT_FALSE(map_.contains("key"));

  // arrange + act + assert
  map_.erase("nonexistent");
}

TEST_F(MapTest, Clear) {
  // arrange
  map_.insert("a", 1);
  map_.insert("b", 2);

  // act
  map_.clear();

  // assert
  ASSERT_TRUE(map_.empty());
}

TEST_F(MapTest, Iterator) {
  // arrange
  map_.insert("a", 1);
  map_.insert("b", 2);

  // act + assert
  json::utils::Map<std::string, int>::Iterator it = map_.begin();
  ASSERT_EQ(it->first, "a");
  ASSERT_EQ(it->second, 1);
  it++;
  ASSERT_EQ(it->first, "b");
  ASSERT_EQ(it->second, 2);
  ++it;
  ASSERT_EQ(it, map_.end());
}

TEST_F(MapTest, ConstIterator) {
  // arrange
  map_.insert("a", 1);
  const json::utils::Map<std::string, int>& cmap = map_;

  // act + assert
  json::utils::Map<std::string, int>::ConstIterator it = cmap.begin();
  it = cmap.cbegin();
  ASSERT_EQ(it->first, "a");
}

TEST_F(MapTest, IteratorFind) {
  // arrange
  map_.insert("key", 1);

  // act
  json::utils::Map<std::string, int>::Iterator it = map_.find("key");

  // assert
  ASSERT_NE(it, map_.end());
  ASSERT_EQ(it->second, 1);

  // act
  it = map_.find("nonexistent");

  // assert
  ASSERT_EQ(it, map_.end());
}

TEST_F(MapTest, Erase) {
  // arrange
  map_.insert("a", 1);
  map_.insert("b", 2);
  map_.insert("c", 3);
  map_.insert("d", 4);

  // act
  map_.erase(map_.begin());

  // assert
  ASSERT_EQ(map_.size(), 3);
}
