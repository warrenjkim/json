#include "utils/rbt.h"

#include <gtest/gtest.h>

#include <boost/log/trivial.hpp>
#include <string>

#include "utils/logger.h"

class RBTreeVerifier {
 public:
  static bool verify_rb_tree_properties(
      const json::utils::RBTree<std::string, int>& tree) {
    const json::utils::rbt::Node<std::string, int>* root = tree.root();
    if (!root) {
      return true;
    }

    int black_height;
    return verify_bst_property(root) && verify_root_property(root) &&
           verify_red_property(root) && verify_black_height(root, black_height);
  }

 private:
  static bool verify_root_property(
      const json::utils::rbt::Node<std::string, int>* root) {
    return (!root || root->color == json::utils::rbt::Color::BLACK);
  }

  static bool verify_red_property(
      const json::utils::rbt::Node<std::string, int>* node) {
    if (node) {
      return true;
    }

    if ((node->color == json::utils::rbt::Color::RED) &&
        ((node->left && node->left->color != json::utils::rbt::Color::BLACK) ||
         (node->right &&
          node->right->color != json::utils::rbt::Color::BLACK))) {
      return false;
    }

    return verify_red_property(node->left) && verify_red_property(node->right);
  }

  static bool verify_black_height(
      const json::utils::rbt::Node<std::string, int>* node, int& black_height) {
    if (!node) {
      black_height = 1;
      return true;
    }

    int left_black_height, right_black_height;
    if (!verify_black_height(node->left, left_black_height) ||
        !verify_black_height(node->right, right_black_height)) {
      return false;
    }

    if (left_black_height != right_black_height) {
      return false;
    }

    black_height = (node->color == json::utils::rbt::Color::BLACK ? 1 : 0) +
                   left_black_height;

    return true;
  }

  static bool verify_bst_property(
      const json::utils::rbt::Node<std::string, int>* node) {
    if (!node) {
      return true;
    }

    if (node->left &&
        (node->left->key >= node->key || !verify_bst_property(node->left))) {
      return false;
    }

    if (node->right &&
        (node->right->key <= node->key || !verify_bst_property(node->right))) {
      return false;
    }

    return true;
  }
};

class RBTreeTest : public ::testing::Test {
 protected:
  void SetUp() override {
    json::utils::init_logging(boost::log::trivial::debug);
  }

  void validate_tree() {
    ASSERT_TRUE(RBTreeVerifier::verify_rb_tree_properties(tree_));
  }

  json::utils::RBTree<std::string, int> tree_;
};

TEST_F(RBTreeTest, InsertAndFind) {
  int* node1 = new int(1);
  int* node2 = new int(1);

  tree_.insert("key1", node1);
  validate_tree();
  tree_.insert("key2", node2);
  validate_tree();

  ASSERT_EQ(tree_.size(), 2);
  ASSERT_EQ(tree_.find("key1")->data, node1);
  ASSERT_EQ(tree_.find("key2")->data, node2);
  ASSERT_EQ(tree_.find("nonexistent"), nullptr);
}

TEST_F(RBTreeTest, RemoveExistingKey) {
  int* node = new int(1);
  tree_.insert("key", node);
  validate_tree();
  ASSERT_EQ(tree_.size(), 1);

  tree_.remove("key");
  validate_tree();
  ASSERT_EQ(tree_.find("key"), nullptr);
  ASSERT_EQ(tree_.size(), 0);
}

TEST_F(RBTreeTest, RemoveNonExistentKey) {
  int* node = new int(1);
  tree_.insert("key", node);
  validate_tree();
  ASSERT_EQ(tree_.size(), 1);

  tree_.remove("nonexistent");
  validate_tree();
  ASSERT_EQ(tree_.size(), 1);
  ASSERT_NE(tree_.find("key"), nullptr);
}

TEST_F(RBTreeTest, InsertDuplicateKey) {
  int* node1 = new int(1);
  int* node2 = new int(1);

  tree_.insert("key", node1);
  validate_tree();
  ASSERT_EQ(tree_.size(), 1);

  tree_.insert("key", node2);
  validate_tree();
  ASSERT_EQ(tree_.size(), 1);
  ASSERT_EQ(tree_.find("key")->data, node2);
}

TEST_F(RBTreeTest, EmptyTreeOperations) {
  validate_tree();
  ASSERT_EQ(tree_.size(), 0);
  ASSERT_EQ(tree_.root(), nullptr);
  ASSERT_EQ(tree_.find("any_key"), nullptr);

  tree_.remove("non_existent_key");
  validate_tree();
  ASSERT_EQ(tree_.size(), 0);
}

TEST_F(RBTreeTest, LargeKeyInsertion) {
  std::string large_key(1000, 'a');
  int* node = new int(1);

  tree_.insert(large_key, node);
  validate_tree();
  ASSERT_EQ(tree_.size(), 1);
  ASSERT_EQ(tree_.find(large_key)->data, node);
}

TEST_F(RBTreeTest, Clear) {
  const int N = 10000;
  for (int i = 0; i < N; i++) {
    tree_.insert(std::to_string(i), new int(1));
    validate_tree();
  }
  ASSERT_EQ(tree_.size(), N);

  tree_.clear();
  tree_.remove("");
  ASSERT_FALSE(tree_.root());
  ASSERT_EQ(tree_.size(), 0);
}

TEST_F(RBTreeTest, InsertAndRemoveAll) {
  const int N = 10000;
  std::vector<int> numbers;
  numbers.reserve(N);

  auto next_pseudo_random = [a = 1103515245, c = 12345, m = 1 << 31]() mutable {
    static unsigned int seed = 12345;
    seed = (a * seed + c) % m;
    return seed;
  };

  for (int i = 0; i < N; i++) {
    numbers.push_back(i);
  }

  for (int i = N - 1; i > 0; i--) {
    int j = next_pseudo_random() % (i + 1);
    std::swap(numbers[i], numbers[j]);
  }

  for (int i = 0; i < N; i++) {
    tree_.insert(std::to_string(numbers[i]), new int(1));
    validate_tree();
    ASSERT_EQ(tree_.size(), i + 1);
  }

  for (int i = N - 1; i > 0; i--) {
    int j = next_pseudo_random() % (i + 1);
    std::swap(numbers[i], numbers[j]);
  }

  for (int i = 0; i < N; i++) {
    tree_.remove(std::to_string(numbers[i]));
    validate_tree();
    ASSERT_EQ(tree_.size(), N - i - 1);
  }

  ASSERT_EQ(tree_.size(), 0);
  ASSERT_EQ(tree_.root(), nullptr);
}

TEST_F(RBTreeTest, StressTestRandomOperations) {
  const int N = 10000;
  auto next_pseudo_random = [a = 1103515245, c = 12345, m = 1 << 31]() mutable {
    static unsigned int seed = 54321;
    seed = (a * seed + c) % m;
    return seed;
  };

  std::set<std::string> unique_keys;
  for (int i = 0; i < N; i++) {
    int operation = next_pseudo_random() % 3;
    std::string key = std::to_string(next_pseudo_random() % (N / 10));

    switch (operation) {
      case 0: {  // insert
        auto [_, inserted] = unique_keys.insert(key);
        tree_.insert(key, new int(1));
      } break;
      case 1:  // remove
        if (!unique_keys.empty()) {
          auto it = unique_keys.begin();
          std::advance(it, next_pseudo_random() % unique_keys.size());
          tree_.remove(*it);
          unique_keys.erase(it);
        }
        break;
      case 2:  // find
        tree_.find(key);
        break;
    }

    validate_tree();
    ASSERT_EQ(tree_.size(), unique_keys.size());
  }
}

TEST_F(RBTreeTest, OperatorBracketTest) {
  tree_["key1"] = 10;
  validate_tree();
  ASSERT_EQ(tree_.size(), 1);
  ASSERT_EQ(tree_["key1"], 10);

  tree_["key2"] = 20;
  validate_tree();
  ASSERT_EQ(tree_.size(), 2);
  ASSERT_EQ(tree_["key2"], 20);

  tree_["key1"] = 15;
  validate_tree();
  ASSERT_EQ(tree_.size(), 2);
  ASSERT_EQ(tree_["key1"], 15);

  int value = tree_["key3"];
  validate_tree();
  ASSERT_EQ(tree_.size(), 3);
  ASSERT_EQ(value, 0);

  const auto& const_tree = tree_;
  ASSERT_EQ(const_tree["key1"], 15);
  ASSERT_EQ(const_tree["key2"], 20);
  ASSERT_EQ(const_tree["key3"], 0);

  ASSERT_THROW(const_tree["key4"], std::out_of_range);
  ASSERT_EQ(tree_.size(), 3);

  tree_["key4"] = tree_["key5"] = 50;
  validate_tree();
  ASSERT_EQ(tree_.size(), 5);
  ASSERT_EQ(tree_["key4"], 50);
  ASSERT_EQ(tree_["key5"], 50);

  value = tree_["key1"];
  ASSERT_EQ(value, 15);
  ASSERT_EQ(tree_.size(), 5);
}
