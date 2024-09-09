#include "utils/rbt.h"

#include <gtest/gtest.h>

#include <boost/log/trivial.hpp>
#include <string>

#include "nodes/node.h"
#include "nodes/object_node.h"
#include "utils/logger.h"

class RBTreeVerifier {
 public:
  static bool verify_rb_tree_properties(const json::utils::RBTree& tree) {
    const json::utils::RBTree::Node* root = tree.root();
    if (!root) {
      return true;
    }

    int black_height;
    return verify_bst_property(root) && verify_root_property(root) &&
           verify_red_property(root) && verify_black_height(root, black_height);
  }

 private:
  static bool verify_root_property(const json::utils::RBTree::Node* root) {
    return (!root || root->color == json::utils::rbt::Color::BLACK);
  }

  static bool verify_red_property(const json::utils::RBTree::Node* node) {
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

  static bool verify_black_height(const json::utils::RBTree::Node* node,
                                  int& black_height) {
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

  static bool verify_bst_property(const json::utils::RBTree::Node* node) {
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
    ASSERT_TRUE(RBTreeVerifier::verify_rb_tree_properties(tree));
  }

  json::utils::RBTree tree;
};

TEST_F(RBTreeTest, InsertAndFind) {
  json::Node* node1 = new json::ObjectNode();
  json::Node* node2 = new json::ObjectNode();

  tree.insert("key1", node1);
  validate_tree();
  tree.insert("key2", node2);
  validate_tree();

  ASSERT_EQ(tree.size(), 2);
  ASSERT_EQ(tree.find("key1")->data, node1);
  ASSERT_EQ(tree.find("key2")->data, node2);
  ASSERT_EQ(tree.find("nonexistent"), nullptr);
}

TEST_F(RBTreeTest, RemoveExistingKey) {
  json::Node* node = new json::ObjectNode();
  tree.insert("key", node);
  validate_tree();
  ASSERT_EQ(tree.size(), 1);

  tree.remove("key");
  validate_tree();
  ASSERT_EQ(tree.find("key"), nullptr);
  ASSERT_EQ(tree.size(), 0);
}

TEST_F(RBTreeTest, RemoveNonExistentKey) {
  json::Node* node = new json::ObjectNode();
  tree.insert("key", node);
  validate_tree();
  ASSERT_EQ(tree.size(), 1);

  tree.remove("nonexistent");
  validate_tree();
  ASSERT_EQ(tree.size(), 1);
  ASSERT_NE(tree.find("key"), nullptr);
}

TEST_F(RBTreeTest, InsertDuplicateKey) {
  json::Node* node1 = new json::ObjectNode();
  json::Node* node2 = new json::ObjectNode();

  tree.insert("key", node1);
  validate_tree();
  ASSERT_EQ(tree.size(), 1);

  tree.insert("key", node2);
  validate_tree();
  ASSERT_EQ(tree.size(), 1);
  ASSERT_EQ(tree.find("key")->data, node2);
}

TEST_F(RBTreeTest, EmptyTreeOperations) {
  validate_tree();
  ASSERT_EQ(tree.size(), 0);
  ASSERT_EQ(tree.root(), nullptr);
  ASSERT_EQ(tree.find("any_key"), nullptr);

  tree.remove("non_existent_key");
  validate_tree();
  ASSERT_EQ(tree.size(), 0);
}

TEST_F(RBTreeTest, LargeKeyInsertion) {
  std::string large_key(1000, 'a');
  json::Node* node = new json::ObjectNode();

  tree.insert(large_key, node);
  validate_tree();
  ASSERT_EQ(tree.size(), 1);
  ASSERT_EQ(tree.find(large_key)->data, node);
}

TEST_F(RBTreeTest, Clear) {
  const int N = 10000;
  for (int i = 0; i < N; i++) {
    tree.insert(std::to_string(i), new json::ObjectNode());
    validate_tree();
  }
  ASSERT_EQ(tree.size(), N);

  tree.clear();
  tree.remove("");
  ASSERT_FALSE(tree.root());
  ASSERT_EQ(tree.size(), 0);
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
    tree.insert(std::to_string(numbers[i]), new json::ObjectNode());
    validate_tree();
    ASSERT_EQ(tree.size(), i + 1);
  }

  for (int i = N - 1; i > 0; i--) {
    int j = next_pseudo_random() % (i + 1);
    std::swap(numbers[i], numbers[j]);
  }

  for (int i = 0; i < N; i++) {
    tree.remove(std::to_string(numbers[i]));
    validate_tree();
    ASSERT_EQ(tree.size(), N - i - 1);
  }

  ASSERT_EQ(tree.size(), 0);
  ASSERT_EQ(tree.root(), nullptr);
}

TEST_F(RBTreeTest, AlternatingInsertAndRemove) {
  const int N = 10000;
  int expected_size = 0;
  for (int i = 0; i < N; i++) {
    tree.insert(std::to_string(i), new json::ObjectNode());
    expected_size++;
    validate_tree();
    ASSERT_EQ(tree.size(), expected_size);

    if (i % 2 == 0 && i > 0) {
      tree.remove(std::to_string(i / 2));
      expected_size--;
      validate_tree();
      ASSERT_EQ(tree.size(), expected_size);
    }
  }

  ASSERT_EQ(tree.size(), expected_size);
  ASSERT_EQ(expected_size, N - (N / 2) + 1);
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
    int operation = next_pseudo_random() % 3;  // 0: insert, 1: remove, 2: find
    std::string key = std::to_string(next_pseudo_random() % (N / 10));

    switch (operation) {
      case 0:  // insert
      {
        auto [_, inserted] = unique_keys.insert(key);
        if (inserted) {
          tree.insert(key, new json::ObjectNode());
        } else {
          // Key already exists, replace the value
          tree.insert(key, new json::ObjectNode());
        }
      } break;
      case 1:  // remove
        if (!unique_keys.empty()) {
          auto it = unique_keys.begin();
          std::advance(it, next_pseudo_random() % unique_keys.size());
          tree.remove(*it);
          unique_keys.erase(it);
        }
        break;
      case 2:  // find
        tree.find(key);
        break;
    }

    validate_tree();
    ASSERT_EQ(tree.size(), unique_keys.size());
  }
}
