#include "utils/rbt.h"

#include <gtest/gtest.h>

#include <vector>

class RBTreeTest : public ::testing::Test {
 protected:
  void validate_tree_properties(const json::utils::RBTree<int>::Node* root) {
    if (!root) {
      return;
    }

    if (!root->parent) {
      ASSERT_EQ(root->color, json::utils::rbt::Color::BLACK);
    }

    if (root->color == json::utils::rbt::Color::RED) {
      if (root->left) {
        ASSERT_EQ(root->left->color, json::utils::rbt::Color::BLACK);
      }

      if (root->right) {
        ASSERT_EQ(root->right->color, json::utils::rbt::Color::BLACK);
      }
    }

    if (root->left) {
      ASSERT_EQ(root->left->parent, root);
    }

    if (root->right) {
      ASSERT_EQ(root->right->parent, root);
    }

    if (root->left) {
      ASSERT_LT(root->left->data, root->data);
    }

    if (root->right) {
      ASSERT_LT(root->data, root->right->data);
    }

    int left_black_height = black_height(root->left);
    int right_black_height = black_height(root->right);
    ASSERT_EQ(left_black_height, right_black_height)
        << "Black height violation at node " << root->data;

    validate_tree_properties(root->left);
    validate_tree_properties(root->right);
  }

  int black_height(const json::utils::RBTree<int>::Node* node) {
    if (!node) {
      return 1;
    }

    int height = black_height(node->left);

    return node->color == json::utils::rbt::Color::BLACK ? height + 1 : height;
  }

  json::utils::RBTree<int> tree_;
};

TEST_F(RBTreeTest, CopyConstructor) {
  // arrange
  tree_.insert(1);
  tree_.insert(2);

  // act
  json::utils::RBTree<int> copy_tree(tree_);

  // assert
  ASSERT_EQ(copy_tree.size(), tree_.size());
  ASSERT_NE(copy_tree.root(), tree_.root());
  ASSERT_NE(copy_tree.find(1), nullptr);
  ASSERT_NE(copy_tree.find(2), nullptr);
  validate_tree_properties(copy_tree.root());
}

TEST_F(RBTreeTest, MoveConstructor) {
  // arrange
  tree_.insert(1);
  json::utils::RBTree<int>::Node* original_root = tree_.root();
  size_t original_size = tree_.size();

  // act
  json::utils::RBTree<int> moved_tree(std::move(tree_));

  // assert
  ASSERT_EQ(tree_.root(), nullptr);
  ASSERT_EQ(tree_.size(), 0);
  ASSERT_EQ(moved_tree.root(), original_root);
  ASSERT_EQ(moved_tree.size(), original_size);
  validate_tree_properties(moved_tree.root());
}

TEST_F(RBTreeTest, CopyAssignment) {
  // arrange
  tree_.insert(1);
  tree_.insert(2);
  json::utils::RBTree<int> other;
  other.insert(3);

  // act
  other = tree_;

  // assert
  ASSERT_EQ(other.size(), tree_.size());
  ASSERT_NE(other.root(), tree_.root());
  ASSERT_NE(other.find(1), nullptr);
  ASSERT_NE(other.find(2), nullptr);
  ASSERT_EQ(other.find(3), nullptr);
  validate_tree_properties(other.root());
}

TEST_F(RBTreeTest, MoveAssignment) {
  // arrange
  tree_.insert(1);
  json::utils::RBTree<int>::Node* original_root = tree_.root();
  size_t original_size = tree_.size();
  json::utils::RBTree<int> other;
  other.insert(2);

  // act
  other = std::move(tree_);

  // assert
  ASSERT_EQ(tree_.root(), nullptr);
  ASSERT_EQ(tree_.size(), 0);
  ASSERT_EQ(other.root(), original_root);
  ASSERT_EQ(other.size(), original_size);
  ASSERT_EQ(other.find(2), nullptr);
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
    ASSERT_NE(tree_.find(node), nullptr);
    ASSERT_EQ(tree_.find(node)->data, node);
    validate_tree_properties(tree_.root());
  }

  // act
  tree_.insert(20);

  ASSERT_NE(tree_.find(20), nullptr);
  ASSERT_EQ(tree_.find(20)->data, 20);
  ASSERT_EQ(tree_.size(), nodes.size() + 1);
  validate_tree_properties(tree_.root());
}

// TODO(ASCII drawing)
TEST_F(RBTreeTest, InsertLeftLeaning) {
  // arrange
  std::vector<int> nodes = {50, 20, 60, 10, 30, 5, 8};
  for (const int node : nodes) {
    tree_.insert(node);
    ASSERT_NE(tree_.find(node), nullptr);
    ASSERT_EQ(tree_.find(node)->data, node);
  }

  // act
  tree_.insert(4);

  // assert
  ASSERT_NE(tree_.find(4), nullptr);
  ASSERT_EQ(tree_.find(4)->data, 4);
  ASSERT_EQ(tree_.size(), nodes.size() + 1);
  validate_tree_properties(tree_.root());
}

// TODO(ASCII drawing)
TEST_F(RBTreeTest, InsertRightLeaning) {
  // arrange
  std::vector<int> nodes = {10, 30, 5, 40, 20, 50, 45};
  for (const int node : nodes) {
    tree_.insert(node);
    ASSERT_NE(tree_.find(node), nullptr);
    ASSERT_EQ(tree_.find(node)->data, node);
  }

  // act
  tree_.insert(60);

  // assert
  ASSERT_NE(tree_.find(60), nullptr);
  ASSERT_EQ(tree_.find(60)->data, 60);
  ASSERT_EQ(tree_.size(), nodes.size() + 1);
  validate_tree_properties(tree_.root());
}

TEST_F(RBTreeTest, InsertDuplicate) {
  tree_.insert(1);
  size_t original_size = tree_.size();

  tree_.insert(1);

  ASSERT_EQ(tree_.size(), original_size);
  validate_tree_properties(tree_.root());
}

// TODO(Add deletion tests)
