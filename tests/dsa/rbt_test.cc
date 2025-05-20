#include "warren/internal/dsa/rbt.h"

#include <vector>

#include "gtest/gtest.h"

class RBTreeTest : public ::testing::Test {
 protected:
  void validate_tree_properties(const json::dsa::RBTree<int>::Node* root) {
    if (!root) {
      return;
    }

    if (!root->parent) {
      EXPECT_EQ(root->color, json::dsa::rbt::Color::BLACK);
    }

    if (root->color == json::dsa::rbt::Color::RED) {
      if (root->left) {
        EXPECT_EQ(root->left->color, json::dsa::rbt::Color::BLACK);
      }

      if (root->right) {
        EXPECT_EQ(root->right->color, json::dsa::rbt::Color::BLACK);
      }
    }

    if (root->left) {
      EXPECT_EQ(root->left->parent, root);
    }

    if (root->right) {
      EXPECT_EQ(root->right->parent, root);
    }

    if (root->left) {
      EXPECT_LT(root->left->data, root->data);
    }

    if (root->right) {
      EXPECT_LT(root->data, root->right->data);
    }

    int left_black_height = black_height(root->left);
    int right_black_height = black_height(root->right);
    EXPECT_EQ(left_black_height, right_black_height)
        << "Black height violation at node " << root->data;

    validate_tree_properties(root->left);
    validate_tree_properties(root->right);
  }

  int black_height(const json::dsa::RBTree<int>::Node* node) {
    if (!node) {
      return 1;
    }

    int height = black_height(node->left);

    return node->color == json::dsa::rbt::Color::BLACK ? height + 1 : height;
  }

  json::dsa::RBTree<int> tree_;
};

TEST_F(RBTreeTest, CopyConstructor) {
  // arrange
  tree_.insert(1);
  tree_.insert(2);

  // act
  json::dsa::RBTree<int> copy_tree(tree_);

  // assert
  EXPECT_EQ(copy_tree.size(), tree_.size());
  EXPECT_NE(copy_tree.root(), tree_.root());
  EXPECT_NE(copy_tree.find(1), nullptr);
  EXPECT_NE(copy_tree.find(2), nullptr);
  validate_tree_properties(copy_tree.root());
}

TEST_F(RBTreeTest, MoveConstructor) {
  // arrange
  tree_.insert(1);
  json::dsa::RBTree<int>::Node* original_root = tree_.root();
  size_t original_size = tree_.size();

  // act
  json::dsa::RBTree<int> moved_tree(std::move(tree_));

  // assert
  EXPECT_EQ(tree_.root(), nullptr);
  EXPECT_TRUE(tree_.empty());
  EXPECT_EQ(moved_tree.root(), original_root);
  EXPECT_EQ(moved_tree.size(), original_size);
  validate_tree_properties(moved_tree.root());
}

TEST_F(RBTreeTest, CopyAssignment) {
  // arrange
  tree_.insert(1);
  tree_.insert(2);
  json::dsa::RBTree<int> other;
  other.insert(3);

  // act
  other = tree_;

  // assert
  EXPECT_EQ(other.size(), tree_.size());
  EXPECT_NE(other.root(), tree_.root());
  EXPECT_NE(other.find(1), nullptr);
  EXPECT_NE(other.find(2), nullptr);
  EXPECT_EQ(other.find(3), nullptr);
  validate_tree_properties(other.root());
}

TEST_F(RBTreeTest, MoveAssignment) {
  // arrange
  tree_.insert(1);
  json::dsa::RBTree<int>::Node* original_root = tree_.root();
  size_t original_size = tree_.size();
  json::dsa::RBTree<int> other;
  other.insert(2);

  // act
  other = std::move(tree_);

  // assert
  EXPECT_EQ(tree_.root(), nullptr);
  EXPECT_TRUE(tree_.empty());
  EXPECT_EQ(other.root(), original_root);
  EXPECT_EQ(other.size(), original_size);
  EXPECT_EQ(other.find(2), nullptr);
  validate_tree_properties(other.root());
}

TEST_F(RBTreeTest, InsertRecolor) {
  //          10B
  //          / \
  //         /   \
  //        5B   15B
  //               \
  //               20R
  std::vector<int> nodes = {10, 5, 15};

  for (const int node : nodes) {
    tree_.insert(node);
    EXPECT_NE(tree_.find(node), nullptr);
    EXPECT_EQ(tree_.find(node)->data, node);
    validate_tree_properties(tree_.root());
  }

  // act
  tree_.insert(20);

  EXPECT_NE(tree_.find(20), nullptr);
  EXPECT_EQ(tree_.find(20)->data, 20);
  EXPECT_EQ(tree_.size(), nodes.size() + 1);
  validate_tree_properties(tree_.root());
}

// TODO(ASCII drawing)
TEST_F(RBTreeTest, InsertLeftLeaning) {
  // arrange
  std::vector<int> nodes = {50, 20, 60, 10, 30, 5, 8};
  for (const int node : nodes) {
    tree_.insert(node);
    EXPECT_NE(tree_.find(node), nullptr);
    EXPECT_EQ(tree_.find(node)->data, node);
  }

  // act
  tree_.insert(4);

  // assert
  EXPECT_NE(tree_.find(4), nullptr);
  EXPECT_EQ(tree_.find(4)->data, 4);
  EXPECT_EQ(tree_.size(), nodes.size() + 1);
  validate_tree_properties(tree_.root());
}

// TODO(ASCII drawing)
TEST_F(RBTreeTest, InsertRightLeaning) {
  // arrange
  std::vector<int> nodes = {10, 30, 5, 40, 20, 50, 45};
  for (const int node : nodes) {
    tree_.insert(node);
    EXPECT_NE(tree_.find(node), nullptr);
    EXPECT_EQ(tree_.find(node)->data, node);
  }

  // act
  tree_.insert(60);

  // assert
  EXPECT_NE(tree_.find(60), nullptr);
  EXPECT_EQ(tree_.find(60)->data, 60);
  EXPECT_EQ(tree_.size(), nodes.size() + 1);
  validate_tree_properties(tree_.root());
}

TEST_F(RBTreeTest, InsertDuplicate) {
  tree_.insert(1);
  size_t original_size = tree_.size();

  tree_.insert(1);

  EXPECT_EQ(tree_.size(), original_size);
  validate_tree_properties(tree_.root());
}

// TODO(Add deletion tests)
