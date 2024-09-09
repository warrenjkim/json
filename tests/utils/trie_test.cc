#include "utils/trie.h"

#include "gtest/gtest.h"
#include "utils/logger.h"

class TrieTest : public ::testing::Test {
 protected:
  void SetUp() override {
    json::utils::init_logging(boost::log::trivial::debug);
  }
};

TEST_F(TrieTest, InsertAndGet) {
  json::utils::Trie<int> trie;

  ASSERT_FALSE(trie.root());

  int* value1 = new int(10);
  int* value2 = new int(20);

  trie.insert("test", value1);
  trie.insert("example", value2);

  ASSERT_EQ(trie.get("nonexistent"), nullptr);
  ASSERT_EQ(trie.get("ex"), nullptr);
  ASSERT_EQ(trie.get("test"), value1);
  ASSERT_EQ(trie.get("example"), value2);

  delete value1;
  delete value2;
}

TEST_F(TrieTest, Contains) {
  json::utils::Trie<int> trie;
  int* value1 = new int(42);

  trie.insert("hello", value1);
  trie.insert("hel", value1);

  ASSERT_TRUE(trie.contains("hello"));
  ASSERT_FALSE(trie.contains("hell"));
  ASSERT_FALSE(trie.contains("world"));

  delete value1;
}

TEST_F(TrieTest, ContainsPrefix) {
  json::utils::Trie<int> trie;
  int* value = new int(100);

  ASSERT_FALSE(trie.contains_prefix("wrong"));

  trie.insert("prefix", value);

  ASSERT_TRUE(trie.contains_prefix("pre"));
  ASSERT_TRUE(trie.contains_prefix("prefix"));
  ASSERT_FALSE(trie.contains_prefix("wrong"));

  delete value;
}

TEST_F(TrieTest, Remove) {
  json::utils::Trie<int> trie;
  int* value = new int(50);

  trie.insert("delete", value);
  ASSERT_TRUE(trie.contains("delete"));

  trie.remove("delete");
  ASSERT_FALSE(trie.contains("delete"));

  trie.insert("stay", value);
  ASSERT_TRUE(trie.contains("stay"));

  trie.remove("dne");
  ASSERT_TRUE(trie.contains("stay"));

  delete value;
}

TEST_F(TrieTest, ClearTrie) {
  json::utils::Trie<int> trie;
  int* value1 = new int(30);
  int* value2 = new int(40);

  trie.insert("key1", value1);
  trie.insert("key2", value2);

  ASSERT_EQ(trie.size(), 2);

  trie.clear();
  ASSERT_EQ(trie.size(), 0);
  ASSERT_EQ(trie.get("key1"), nullptr);
  ASSERT_EQ(trie.get("key2"), nullptr);

  delete value1;
  delete value2;
}

TEST_F(TrieTest, MoveConstructor) {
  json::utils::Trie<int> trie1;
  int* value1 = new int(5);
  trie1.insert("test1", value1);

  json::utils::Trie<int> trie2(std::move(trie1));

  ASSERT_EQ(trie2.size(), 1);
  ASSERT_EQ(*trie2.get("test1"), 5);
  ASSERT_EQ(trie1.get("test1"), nullptr);

  delete value1;
}

TEST_F(TrieTest, MoveAssignment) {
  json::utils::Trie<int> trie1;
  json::utils::Trie<int> trie2;
  int* value1 = new int(15);
  int* value2 = new int(25);

  trie1.insert("key1", value1);
  trie2.insert("key2", value2);

  trie1 = std::move(trie2);

  ASSERT_EQ(trie1.size(), 1);
  ASSERT_EQ(*trie1.get("key2"), 25);
  ASSERT_EQ(trie2.size(), 0);

  delete value1;
  delete value2;
}

TEST_F(TrieTest, PrefixOverlap) {
  json::utils::Trie<int> trie;
  int* value1 = new int(1);
  int* value2 = new int(2);
  int* value3 = new int(3);

  trie.insert("a", value1);
  trie.insert("ab", value2);
  trie.insert("abc", value3);

  ASSERT_EQ(trie.get("a"), value1);
  ASSERT_EQ(trie.get("ab"), value2);
  ASSERT_EQ(trie.get("abc"), value3);

  trie.remove("ab");
  ASSERT_EQ(trie.get("a"), value1);
  ASSERT_EQ(trie.get("ab"), nullptr);
  ASSERT_EQ(trie.get("abc"), value3);

  delete value1;
  delete value2;
  delete value3;
}

TEST_F(TrieTest, InsertAfterRemoval) {
  json::utils::Trie<int> trie;
  int* value1 = new int(42);
  int* value2 = new int(43);

  trie.insert("test", value1);
  ASSERT_TRUE(trie.contains("test"));

  trie.remove("test");
  ASSERT_FALSE(trie.contains("test"));

  trie.insert("test", value2);
  ASSERT_TRUE(trie.contains("test"));
  ASSERT_EQ(trie.get("test"), value2);

  delete value1;
  delete value2;
}

TEST_F(TrieTest, VisualizeKeys) {
  json::utils::Trie<int> trie;

  trie.insert("cat", new int(1));
  trie.insert("car", new int(2));
  trie.insert("cart", new int(3));
  trie.insert("ex", new int(4));
  trie.insert("example", new int(5));

  std::string visualization = trie.visualize_keys();

  std::string expected =
      "|   |-- e\n"
      "|   |   |-- x (end)\n"
      "|   |       |-- a\n"
      "|   |           |-- m\n"
      "|   |               |-- p\n"
      "|   |                   |-- l\n"
      "|   |                       |-- e (end)\n"
      "|   |-- c\n"
      "|       |-- a\n"
      "|           |-- r (end)\n"
      "|           |   |-- t (end)\n"
      "|           |-- t (end)\n";

  ASSERT_EQ(visualization, expected);

  delete trie.get("cat");
  delete trie.get("car");
  delete trie.get("dog");
  delete trie.get("doge");
}
