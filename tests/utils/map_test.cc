#include "utils/map.h"

#include <gtest/gtest.h>

#include <boost/log/trivial.hpp>
#include <set>
#include <string>
#include <vector>

#include "utils/logger.h"

class MapVerifier {
 public:
  static bool verify_rb_tree_properties(
      const json::utils::Map<std::string, int>& tree) {
    const json::utils::Map<std::string, int>::Node* root = tree.root();
    if (!root) {
      return true;
    }

    int black_height;
    return verify_bst_property(root) && verify_root_property(root) &&
           verify_red_property(root) && verify_black_height(root, black_height);
  }

 private:
  static bool verify_root_property(
      const json::utils::Map<std::string, int>::Node* root) {
    return (!root || root->color == json::utils::map::Color::BLACK);
  }

  static bool verify_red_property(
      const json::utils::Map<std::string, int>::Node* node) {
    if (!node) {
      return true;
    }

    if ((node->color == json::utils::map::Color::RED) &&
        ((node->left && node->left->color != json::utils::map::Color::BLACK) ||
         (node->right &&
          node->right->color != json::utils::map::Color::BLACK))) {
      return false;
    }

    return verify_red_property(node->left) && verify_red_property(node->right);
  }

  static bool verify_black_height(
      const json::utils::Map<std::string, int>::Node* node, int& black_height) {
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

    black_height = (node->color == json::utils::map::Color::BLACK ? 1 : 0) +
                   left_black_height;

    return true;
  }

  static bool verify_bst_property(
      const json::utils::Map<std::string, int>::Node* node) {
    if (!node) {
      return true;
    }

    if (node->left && (node->left->data.first >= node->data.first ||
                       !verify_bst_property(node->left))) {
      return false;
    }

    if (node->right && (node->right->data.first <= node->data.first ||
                        !verify_bst_property(node->right))) {
      return false;
    }

    return true;
  }
};

class MapTest : public ::testing::Test {
 protected:
  void SetUp() override {
    json::utils::init_logging(boost::log::trivial::debug);
  }

  void validate_tree() {
    ASSERT_TRUE(MapVerifier::verify_rb_tree_properties(tree_));
  }

  json::utils::Map<std::string, int> tree_;
};

TEST_F(MapTest, EmptyAndContains) {
  ASSERT_TRUE(tree_.empty());
  tree_.insert("key", 1);
  ASSERT_FALSE(tree_.empty());
  ASSERT_TRUE(tree_.contains("key"));
  ASSERT_FALSE(tree_.contains("key2"));
}

TEST_F(MapTest, RemoveRootNode) {
  tree_.insert("key2", 2);
  tree_.insert("key1", 1);
  tree_.insert("key3", 3);
  validate_tree();

  tree_.remove("key2");
  validate_tree();

  const auto& const_tree = tree_;
  ASSERT_FALSE(const_tree.get("key2").has_value());
  ASSERT_EQ(tree_.size(), 2);
  ASSERT_TRUE(const_tree.get("key1").has_value());
  ASSERT_TRUE(tree_.get("key3").has_value());
}

TEST_F(MapTest, RemoveNodeWithOneChild) {
  tree_.insert("key1", 1);
  tree_.insert("key2", 2);
  tree_.insert("key3", 3);
  validate_tree();

  tree_.remove("key2");
  validate_tree();

  ASSERT_FALSE(tree_.get("key2").has_value());
  ASSERT_EQ(tree_.size(), 2);
}

TEST_F(MapTest, RemoveNodeWithTwoChildren) {
  tree_.insert("key2", 2);
  tree_.insert("key1", 1);
  tree_.insert("key3", 3);
  tree_.insert("key0", 0);
  validate_tree();

  tree_.remove("key2");
  validate_tree();

  ASSERT_FALSE(tree_.get("key2").has_value());
  ASSERT_EQ(tree_.size(), 3);
}

TEST_F(MapTest, LeftLeftRotationTest) {
  tree_.insert("key3", 3);
  tree_.insert("key2", 2);
  tree_.insert("key1", 1);
  validate_tree();
}

TEST_F(MapTest, LeftRightRotationTest) {
  tree_.insert("key3", 3);
  tree_.insert("key1", 1);
  tree_.insert("key2", 2);
  validate_tree();
}

TEST_F(MapTest, RightRightRotationTest) {
  tree_.insert("key1", 1);
  tree_.insert("key2", 2);
  tree_.insert("key3", 3);
  validate_tree();
}

TEST_F(MapTest, RightLeftRotationTest) {
  tree_.insert("key1", 1);
  tree_.insert("key3", 3);
  tree_.insert("key2", 2);
  validate_tree();
}

TEST_F(MapTest, RecoloringTest) {
  tree_.insert("key5", 5);
  tree_.insert("key3", 3);
  tree_.insert("key7", 7);
  tree_.insert("key2", 2);
  tree_.insert("key4", 4);
  tree_.insert("key6", 6);
  tree_.insert("key8", 8);
  validate_tree();
}

TEST_F(MapTest, DoubleBlackFixupTest) {
  tree_.insert("key5", 5);
  tree_.insert("key3", 3);
  tree_.insert("key7", 7);
  tree_.insert("key2", 2);
  tree_.insert("key4", 4);
  tree_.insert("key6", 6);
  tree_.insert("key8", 8);
  validate_tree();

  tree_.remove("key2");
  validate_tree();
}

TEST_F(MapTest, InsertAndFind) {
  tree_.insert("key1", 1);
  validate_tree();
  tree_.insert("key2", 2);
  validate_tree();

  ASSERT_EQ(tree_.size(), 2);
  ASSERT_EQ(tree_.get("key1"), 1);
  ASSERT_EQ(tree_.get("key2"), 2);
  ASSERT_FALSE(tree_.get("nonexistent").has_value());
}

TEST_F(MapTest, RemoveExistingKey) {
  tree_.insert("key", 1);
  validate_tree();
  ASSERT_EQ(tree_.size(), 1);

  tree_.remove("key");
  validate_tree();
  ASSERT_FALSE(tree_.get("key").has_value());
  ASSERT_EQ(tree_.size(), 0);
}

TEST_F(MapTest, RemoveNonExistentKey) {
  tree_.insert("key", 1);
  validate_tree();
  ASSERT_EQ(tree_.size(), 1);

  tree_.remove("nonexistent");
  validate_tree();
  ASSERT_EQ(tree_.size(), 1);
  ASSERT_TRUE(tree_.get("key").has_value());
}

TEST_F(MapTest, InsertDuplicateKey) {
  tree_.insert("key", 1);
  validate_tree();
  ASSERT_EQ(tree_.size(), 1);

  tree_.insert("key", 2);
  validate_tree();
  ASSERT_EQ(tree_.size(), 1);
  ASSERT_EQ(tree_.get("key"), 2);
}

TEST_F(MapTest, EmptyTreeOperations) {
  validate_tree();
  ASSERT_EQ(tree_.size(), 0);
  ASSERT_EQ(tree_.root(), nullptr);
  ASSERT_FALSE(tree_.get("any_key").has_value());

  tree_.remove("non_existent_key");
  validate_tree();
  ASSERT_EQ(tree_.size(), 0);
}

TEST_F(MapTest, LargeKeyInsertion) {
  std::string large_key(1000, 'a');
  tree_.insert(large_key, 1);
  validate_tree();
  ASSERT_EQ(tree_.size(), 1);
  ASSERT_EQ(tree_.get(large_key), 1);
}

TEST_F(MapTest, Clear) {
  const int N = 10000;
  for (int i = 0; i < N; i++) {
    tree_.insert(std::to_string(i), i);
    validate_tree();
  }
  ASSERT_EQ(tree_.size(), N);

  tree_.clear();
  ASSERT_FALSE(tree_.root());
  ASSERT_EQ(tree_.size(), 0);
}

TEST_F(MapTest, StressTestRandomOperations) {
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
        tree_.insert(key, i);
      } break;
      case 1:  // remove
        if (!unique_keys.empty()) {
          auto it = unique_keys.begin();
          std::advance(it, next_pseudo_random() % unique_keys.size());
          tree_.remove(*it);
          unique_keys.erase(it);
        }
        break;
      case 2:  // get
        tree_.get(key);
        break;
    }

    validate_tree();
    ASSERT_EQ(tree_.size(), unique_keys.size());
  }
}

TEST_F(MapTest, OperatorBracketTest) {
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

  ASSERT_EQ(const_tree["key4"], 0);
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

TEST_F(MapTest, CopyConstructor) {
  tree_.insert("key1", 1);
  tree_.insert("key2", 2);
  tree_.insert("key3", 3);
  validate_tree();
  ASSERT_EQ(tree_.size(), 3);

  json::utils::Map<std::string, int> copy_tree(tree_);
  ASSERT_EQ(copy_tree.size(), 3);
  ASSERT_EQ(copy_tree.get("key1"), 1);
  ASSERT_EQ(copy_tree.get("key2"), 2);
  ASSERT_EQ(copy_tree.get("key3"), 3);

  tree_.remove("key1");
  validate_tree();
  ASSERT_FALSE(tree_.get("key1").has_value());
  ASSERT_TRUE(copy_tree.get("key1").has_value());
}

TEST_F(MapTest, CopyConstructorNoOp) {
  ASSERT_EQ(tree_.size(), 0);

  json::utils::Map<std::string, int> copy_tree(tree_);
  ASSERT_EQ(copy_tree.size(), 0);

  tree_["key1"] = 1;
  validate_tree();
  ASSERT_TRUE(tree_.get("key1").has_value());
  ASSERT_FALSE(copy_tree.get("key1").has_value());
}

TEST_F(MapTest, CopyAssignmentOperator) {
  tree_.insert("key1", 1);
  tree_.insert("key2", 2);
  tree_.insert("key3", 3);
  validate_tree();
  ASSERT_EQ(tree_.size(), 3);

  json::utils::Map<std::string, int> copy_tree;
  copy_tree = tree_;
  ASSERT_EQ(copy_tree.size(), 3);
  ASSERT_EQ(copy_tree.get("key1"), 1);
  ASSERT_EQ(copy_tree.get("key2"), 2);
  ASSERT_EQ(copy_tree.get("key3"), 3);

  // Modify the original tree and ensure the copy is unaffected
  tree_.remove("key1");
  validate_tree();
  ASSERT_FALSE(tree_.get("key1").has_value());
  ASSERT_TRUE(copy_tree.get("key1").has_value());
}

TEST_F(MapTest, MoveConstructor) {
  tree_.insert("key1", 1);
  tree_.insert("key2", 2);
  tree_.insert("key3", 3);
  validate_tree();
  ASSERT_EQ(tree_.size(), 3);

  json::utils::Map<std::string, int> moved_tree(std::move(tree_));
  ASSERT_EQ(moved_tree.size(), 3);
  ASSERT_EQ(moved_tree.get("key1"), 1);
  ASSERT_EQ(moved_tree.get("key2"), 2);
  ASSERT_EQ(moved_tree.get("key3"), 3);

  // Original tree should be empty
  ASSERT_EQ(tree_.size(), 0);
  ASSERT_FALSE(tree_.get("key1").has_value());
}

TEST_F(MapTest, MoveAssignmentOperator) {
  tree_.insert("key1", 1);
  tree_.insert("key2", 2);
  tree_.insert("key3", 3);
  validate_tree();
  ASSERT_EQ(tree_.size(), 3);

  json::utils::Map<std::string, int> moved_tree;
  moved_tree = std::move(tree_);
  ASSERT_EQ(moved_tree.size(), 3);
  ASSERT_EQ(moved_tree.get("key1"), 1);
  ASSERT_EQ(moved_tree.get("key2"), 2);
  ASSERT_EQ(moved_tree.get("key3"), 3);

  ASSERT_EQ(tree_.size(), 0);
  ASSERT_FALSE(tree_.get("key1").has_value());
}

TEST_F(MapTest, SwapFunction) {
  tree_.insert("key1", 1);
  tree_.insert("key2", 2);
  tree_.insert("key3", 3);
  validate_tree();
  ASSERT_EQ(tree_.size(), 3);

  json::utils::Map<std::string, int> other_tree;
  other_tree.insert("key4", 4);
  other_tree.insert("key5", 5);
  validate_tree();
  ASSERT_EQ(other_tree.size(), 2);

  tree_.swap(other_tree);
  validate_tree();

  ASSERT_EQ(tree_.size(), 2);
  ASSERT_EQ(tree_.get("key4"), 4);
  ASSERT_EQ(tree_.get("key5"), 5);
  ASSERT_FALSE(tree_.get("key1").has_value());

  ASSERT_EQ(other_tree.size(), 3);
  ASSERT_EQ(other_tree.get("key1"), 1);
  ASSERT_EQ(other_tree.get("key2"), 2);
  ASSERT_EQ(other_tree.get("key3"), 3);
  ASSERT_FALSE(other_tree.get("key4").has_value());
}

TEST_F(MapTest, IteratorBoundsTest) {
  auto it = tree_.begin();
  it--;
  ASSERT_EQ(it, tree_.end());

  it = tree_.end();
  ASSERT_THROW(it++, std::out_of_range);
}

TEST_F(MapTest, IteratorTest) {
  tree_.insert("key1", 1);
  tree_.insert("key2", 2);
  tree_.insert("key3", 3);
  validate_tree();

  std::vector<std::string> keys;
  for (json::utils::Pair<const std::string, int>& x : tree_) {
    keys.push_back(x.first);
  }

  ASSERT_EQ(keys.size(), 3);
  ASSERT_EQ(keys[0], "key1");
  ASSERT_EQ(keys[1], "key2");
  ASSERT_EQ(keys[2], "key3");
}

TEST_F(MapTest, ReverseIteratorTest) {
  tree_.insert("key1", 1);
  tree_.insert("key2", 2);
  tree_.insert("key3", 3);
  validate_tree();

  auto it = tree_.end();
  std::vector<std::string> keys;
  while (it != tree_.begin()) {
    --it;
    keys.push_back(it->first);
  }

  ASSERT_EQ(keys.size(), 3);
  ASSERT_EQ(keys[0], "key3");
  ASSERT_EQ(keys[1], "key2");
  ASSERT_EQ(keys[2], "key1");
}

TEST_F(MapTest, IteratorDereferenceEnd) {
  tree_.insert("key1", 1);
  validate_tree();

  auto it = tree_.end();
  ASSERT_THROW(*it, std::out_of_range);
}

TEST_F(MapTest, ConstIteratorBoundsTest) {
  auto it = tree_.cbegin();
  it--;
  ASSERT_EQ(it, tree_.cend());

  it = tree_.cend();
  ASSERT_THROW(it++, std::out_of_range);
}

TEST_F(MapTest, ConstIteratorTest) {
  tree_.insert("key1", 1);
  tree_.insert("key2", 2);
  tree_.insert("key3", 3);
  validate_tree();

  const auto& const_tree = tree_;
  std::vector<std::string> keys;
  for (auto it = tree_.cbegin(); it != tree_.cend(); it++) {
    keys.push_back(it->first);
  }

  ASSERT_EQ(keys.size(), 3);
  ASSERT_EQ(keys[0], "key1");
  ASSERT_EQ(keys[1], "key2");
  ASSERT_EQ(keys[2], "key3");
}

TEST_F(MapTest, ConstBeginEndTest) {
  tree_.insert("key1", 1);
  tree_.insert("key2", 2);
  validate_tree();

  const auto& const_tree = tree_;
  auto it = const_tree.cbegin();
  json::utils::Map<std::string, int>::ConstIterator const_it =
      const_tree.begin();
  ASSERT_EQ(it->first, "key1");
  ASSERT_EQ(const_it->first, "key1");

  it = const_tree.cend();
  ASSERT_THROW(*it, std::out_of_range);
}

TEST_F(MapTest, FindTest) {
  tree_.insert("key1", 1);
  tree_.insert("key2", 2);
  tree_.insert("key3", 3);
  validate_tree();

  auto it = tree_.find("key2");
  ASSERT_NE(it, tree_.end());
  ASSERT_EQ(it->first, "key2");
  ASSERT_EQ(it->second, 2);

  it = tree_.find("nonexistent");
  ASSERT_EQ(it, tree_.end());
}

TEST_F(MapTest, ConstFindTest) {
  tree_.insert("key1", 1);
  tree_.insert("key2", 2);
  validate_tree();

  const auto& const_tree = tree_;
  auto it = const_tree.find("key2");
  ASSERT_NE(it, const_tree.end());
  ASSERT_EQ(it->first, "key2");

  it = const_tree.find("nonexistent");
  ASSERT_EQ(it, const_tree.end());
}

TEST_F(MapTest, PredecessorIncorrectInRedBlackTree) {
  tree_.insert("20", 20);
  tree_.insert("10", 10);
  tree_.insert("30", 30);
  tree_.insert("25", 25);
  tree_.insert("23", 23);
  validate_tree();

  auto it = tree_.find("23");
  ASSERT_NE(it, tree_.end());
  ASSERT_EQ(it->first, "23");

  --it;
  ASSERT_EQ(it->first, "20");

  --it;
  ASSERT_EQ(it->first, "10");

  --it;
  ASSERT_EQ(it, tree_.end());
}

TEST_F(MapTest, PredecessorWithParentTraversal) {
  // Construct the tree as per the example:
  //          30
  //         /  \
  //       20    40
  //      /  \
  //     10  25
  //         /
  //       22

  tree_.insert("30", 30);
  tree_.insert("20", 20);
  tree_.insert("40", 40);
  tree_.insert("10", 10);
  tree_.insert("25", 25);
  tree_.insert("22", 22);
  validate_tree();
  for (auto x : tree_) {
    std::cout << x.first << std::endl;
  }

  auto it = tree_.find("22");
  ASSERT_NE(it, tree_.end());
  ASSERT_EQ(it->first, "22");

  --it;
  ASSERT_EQ(it->first, "20");

  --it;
  ASSERT_EQ(it->first, "10");

  --it;
  ASSERT_EQ(it, tree_.end());
}

TEST_F(MapTest, IteratorDecrementAtBegin) {
  tree_.insert("key2", 2);
  tree_.insert("key1", 1);
  tree_.insert("key3", 3);
  validate_tree();

  auto it = tree_.begin();
  ASSERT_EQ(it->first, "key1");

  it--;
  ASSERT_EQ(it, tree_.end());

  it--;
  ASSERT_EQ(it->first, "key3");

  it--;
  ASSERT_EQ(it->first, "key2");

  it--;
  ASSERT_EQ(it->first, "key1");
}

TEST_F(MapTest, IteratorDecrementFromEnd) {
  tree_.insert("key2", 2);
  tree_.insert("key1", 1);
  tree_.insert("key3", 3);
  validate_tree();
  auto it = tree_.end();

  it--;
  ASSERT_EQ(it->first, "key3");

  it--;
  ASSERT_EQ(it->first, "key2");

  it--;
  ASSERT_EQ(it->first, "key1");

  it--;
  ASSERT_EQ(it, tree_.end());
}

TEST_F(MapTest, IteratorDecrementOnEmptyTree) {
  ASSERT_EQ(tree_.size(), 0);

  auto it = tree_.begin();
  ASSERT_EQ(it, tree_.end());

  --it;
  ASSERT_EQ(it, tree_.end());
  ASSERT_THROW(*it, std::out_of_range);
}

TEST_F(MapTest, PredecessorNoLeftChildViaOperator) {
  tree_.insert("key3", 3);
  tree_.insert("key2", 2);
  tree_.insert("key4", 4);
  validate_tree();

  auto it = tree_.find("key2");
  ASSERT_NE(it, tree_.end());
  ASSERT_EQ(it->first, "key2");

  --it;
  ASSERT_EQ(it, tree_.end());
}

TEST_F(MapTest, ConstIteratorDecrementAtBegin) {
  tree_.insert("key2", 2);
  tree_.insert("key1", 1);
  tree_.insert("key3", 3);
  validate_tree();

  auto it = tree_.begin();
  ASSERT_EQ(it->first, "key1");

  it--;
  ASSERT_EQ(it, tree_.end());

  it--;
  ASSERT_EQ(it->first, "key3");

  it--;
  ASSERT_EQ(it->first, "key2");

  it--;
  ASSERT_EQ(it->first, "key1");
}

TEST_F(MapTest, ConstIteratorDecrementFromEnd) {
  tree_.insert("key2", 2);
  tree_.insert("key1", 1);
  tree_.insert("key3", 3);
  validate_tree();
  auto it = tree_.cend();

  it--;
  ASSERT_EQ(it->first, "key3");

  it--;
  ASSERT_EQ(it->first, "key2");

  it--;
  ASSERT_EQ(it->first, "key1");

  it--;
  ASSERT_EQ(it, tree_.cend());
}

TEST_F(MapTest, ConstIteratorDecrementOnEmptyTree) {
  ASSERT_EQ(tree_.size(), 0);

  auto it = tree_.cbegin();
  ASSERT_EQ(it, tree_.cend());

  --it;
  ASSERT_EQ(it, tree_.cend());
  ASSERT_THROW(*it, std::out_of_range);
}

TEST_F(MapTest, ExceptionOnDereferenceInvalidIterator) {
  json::utils::Map<std::string, int>::Iterator it;
  ASSERT_THROW(*it, std::out_of_range);
  ASSERT_THROW(it++, std::out_of_range);
  ASSERT_THROW(it--, std::out_of_range);
}

TEST_F(MapTest, ExceptionOnDereferenceInvalidConstIterator) {
  json::utils::Map<std::string, int>::ConstIterator it;
  ASSERT_THROW(*it, std::out_of_range);
  ASSERT_THROW(it++, std::out_of_range);
  ASSERT_THROW(it--, std::out_of_range);
}

TEST_F(MapTest, Equality) {
  json::utils::Map<std::string, int> lhs;
  lhs.insert("key2", 2);
  lhs.insert("key1", 1);
  lhs.insert("key3", 3);
  lhs.insert("key0", 0);

  json::utils::Map<std::string, int> rhs;
  rhs.insert("key2", 2);
  rhs.insert("key1", 1);
  rhs.insert("key3", 3);
  rhs.insert("key0", 0);

  ASSERT_EQ(lhs, rhs);
}

TEST_F(MapTest, InequalitySize) {
  json::utils::Map<std::string, int> lhs;
  lhs.insert("key2", 2);
  lhs.insert("key1", 1);
  lhs.insert("key3", 3);
  lhs.insert("key0", 0);

  json::utils::Map<std::string, int> rhs;
  rhs.insert("key2", 2);
  rhs.insert("key1", 1);
  rhs.insert("key3", 3);

  ASSERT_NE(lhs, rhs);
}

TEST_F(MapTest, Inequality) {
  json::utils::Map<std::string, int> lhs;
  lhs.insert("key2", 2);
  lhs.insert("key1", 1);
  lhs.insert("key3", 3);
  lhs.insert("key0", 0);

  json::utils::Map<std::string, int> rhs;
  rhs.insert("key2", 2);
  rhs.insert("key1", 1);
  rhs.insert("key3", 3);
  rhs.insert("key4", 0);

  ASSERT_NE(lhs, rhs);
}
