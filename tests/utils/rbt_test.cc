#include "utils/rbt.h"

#include <gtest/gtest.h>

#include <string>

class RBTreeTest : public ::testing::Test {
 protected:
  using Tree = json::utils::RBTree<std::string, int>;
  using Node = Tree::Node;
  using Color = json::utils::rbt::Color;

  void validate_tree_properties(const Node* root) {
    if (!root) return;
    if (!root->parent) {
      ASSERT_EQ(root->color, Color::BLACK);
    }
    if (root->color == Color::RED) {
      if (root->left) ASSERT_EQ(root->left->color, Color::BLACK);
      if (root->right) ASSERT_EQ(root->right->color, Color::BLACK);
    }
    if (root->left) {
      ASSERT_EQ(root->left->parent, root);
    }
    if (root->right) {
      ASSERT_EQ(root->right->parent, root);
    }
    validate_tree_properties(root->left);
    validate_tree_properties(root->right);
  }

  Tree tree_;
};

// Constructor Tests
TEST_F(RBTreeTest, DefaultConstructor) {
  // Assert
  ASSERT_EQ(tree_.root(), nullptr);
  ASSERT_EQ(tree_.size(), 0);
  ASSERT_TRUE(tree_.empty());
}

TEST_F(RBTreeTest, CopyConstructor) {
  // Arrange
  tree_.insert("key1", 1);
  tree_.insert("key2", 2);

  // Act
  Tree copy_tree(tree_);

  // Assert
  ASSERT_EQ(copy_tree.size(), tree_.size());
  ASSERT_NE(copy_tree.root(), tree_.root());  // Different addresses
  ASSERT_EQ(copy_tree.find("key1")->data.second, 1);
  validate_tree_properties(copy_tree.root());
}

TEST_F(RBTreeTest, MoveConstructor) {
  // Arrange
  tree_.insert("key1", 1);
  Node* original_root = tree_.root();

  // Act
  Tree moved_tree(std::move(tree_));

  // Assert
  ASSERT_EQ(tree_.root(), nullptr);  // Original emptied
  ASSERT_EQ(moved_tree.root(), original_root);
  validate_tree_properties(moved_tree.root());
}

// Assignment Operator Tests
TEST_F(RBTreeTest, CopyAssignment) {
  // Arrange
  tree_.insert("key1", 1);
  Tree other;

  // Act
  other = tree_;

  // Assert
  ASSERT_EQ(other.size(), tree_.size());
  ASSERT_NE(other.root(), tree_.root());
  ASSERT_EQ(other.find("key1")->data.second, 1);
  validate_tree_properties(other.root());
}

TEST_F(RBTreeTest, MoveAssignment) {
  // Arrange
  tree_.insert("key1", 1);
  Node* original_root = tree_.root();
  Tree other;

  // Act
  other = std::move(tree_);

  // Assert
  ASSERT_EQ(tree_.root(), nullptr);
  ASSERT_EQ(other.root(), original_root);
  validate_tree_properties(other.root());
}

// Root Access Tests
TEST_F(RBTreeTest, RootAccessNonConst) {
  // Arrange
  tree_.insert("key1", 1);

  // Act
  Node* root = tree_.root();

  // Assert
  ASSERT_NE(root, nullptr);
  ASSERT_EQ(root->data.first, "key1");
}

TEST_F(RBTreeTest, RootAccessConst) {
  // Arrange
  tree_.insert("key1", 1);
  const Tree& const_tree = tree_;

  // Act
  const Node* root = const_tree.root();

  // Assert
  ASSERT_NE(root, nullptr);
  ASSERT_EQ(root->data.first, "key1");
}

// Min/Max Tests
TEST_F(RBTreeTest, MinNonConst) {
  // Arrange
  tree_.insert("b", 2);
  tree_.insert("a", 1);
  tree_.insert("c", 3);

  // Act
  Node* min_node = tree_.min();

  // Assert
  ASSERT_NE(min_node, nullptr);
  ASSERT_EQ(min_node->data.first, "a");
}

TEST_F(RBTreeTest, MinConst) {
  // Arrange
  tree_.insert("b", 2);
  tree_.insert("a", 1);
  const Tree& const_tree = tree_;

  // Act
  const Node* min_node = const_tree.min();

  // Assert
  ASSERT_NE(min_node, nullptr);
  ASSERT_EQ(min_node->data.first, "a");
}

TEST_F(RBTreeTest, MaxNonConst) {
  // Arrange
  tree_.insert("b", 2);
  tree_.insert("a", 1);
  tree_.insert("c", 3);

  // Act
  Node* max_node = tree_.max();

  // Assert
  ASSERT_NE(max_node, nullptr);
  ASSERT_EQ(max_node->data.first, "c");
}

TEST_F(RBTreeTest, MaxConst) {
  // Arrange
  tree_.insert("b", 2);
  tree_.insert("c", 3);
  const Tree& const_tree = tree_;

  // Act
  const Node* max_node = const_tree.max();

  // Assert
  ASSERT_NE(max_node, nullptr);
  ASSERT_EQ(max_node->data.first, "c");
}

// Successor/Predecessor Tests
TEST_F(RBTreeTest, SuccessorNonConst) {
  // Arrange
  tree_.insert("b", 2);
  tree_.insert("a", 1);
  tree_.insert("c", 3);
  Node* node = tree_.find("b");

  // Act
  Node* succ = tree_.successor(node);

  // Assert
  ASSERT_NE(succ, nullptr);
  ASSERT_EQ(succ->data.first, "c");
}

TEST_F(RBTreeTest, SuccessorConst) {
  // Arrange
  tree_.insert("b", 2);
  tree_.insert("c", 3);
  const Tree& const_tree = tree_;
  const Node* node = const_tree.find("b");

  // Act
  const Node* succ = const_tree.successor(node);

  // Assert
  ASSERT_NE(succ, nullptr);
  ASSERT_EQ(succ->data.first, "c");
}

TEST_F(RBTreeTest, PredecessorNonConst) {
  // Arrange
  tree_.insert("b", 2);
  tree_.insert("a", 1);
  Node* node = tree_.find("b");

  // Act
  Node* pred = tree_.predecessor(node);

  // Assert
  ASSERT_NE(pred, nullptr);
  ASSERT_EQ(pred->data.first, "a");
}

TEST_F(RBTreeTest, PredecessorConst) {
  // Arrange
  tree_.insert("b", 2);
  tree_.insert("a", 1);
  const Tree& const_tree = tree_;
  const Node* node = const_tree.find("b");

  // Act
  const Node* pred = const_tree.predecessor(node);

  // Assert
  ASSERT_NE(pred, nullptr);
  ASSERT_EQ(pred->data.first, "a");
}

// Empty and Size Tests
TEST_F(RBTreeTest, EmptyAndSize) {
  // Assert initial
  ASSERT_TRUE(tree_.empty());
  ASSERT_EQ(tree_.size(), 0);

  // Act
  tree_.insert("key1", 1);

  // Assert after insert
  ASSERT_FALSE(tree_.empty());
  ASSERT_EQ(tree_.size(), 1);

  // Act again
  tree_.clear();

  // Assert after clear
  ASSERT_TRUE(tree_.empty());
  ASSERT_EQ(tree_.size(), 0);
}

// Insert/Erase/Clear Tests
TEST_F(RBTreeTest, InsertAndFind) {
  // Act
  tree_.insert("key1", 1);
  tree_.insert("key2", 2);

  // Assert
  ASSERT_EQ(tree_.find("key1")->data.second, 1);
  ASSERT_EQ(tree_.find("key2")->data.second, 2);
  validate_tree_properties(tree_.root());
}

TEST_F(RBTreeTest, EraseExistingKey) {
  // Arrange
  tree_.insert("key1", 1);
  tree_.insert("key2", 2);
  size_t initial_size = tree_.size();

  // Act
  tree_.erase("key1");

  // Assert
  ASSERT_EQ(tree_.size(), initial_size - 1);
  ASSERT_EQ(tree_.find("key1"), nullptr);
  validate_tree_properties(tree_.root());
}

TEST_F(RBTreeTest, Clear) {
  // Arrange
  tree_.insert("key1", 1);
  tree_.insert("key2", 2);

  // Act
  tree_.clear();

  // Assert
  ASSERT_TRUE(tree_.empty());
  ASSERT_EQ(tree_.size(), 0);
  ASSERT_EQ(tree_.root(), nullptr);
}

// Comparison Operator Tests
TEST_F(RBTreeTest, EqualityOperator) {
  // Arrange
  tree_.insert("key1", 1);
  tree_.insert("key2", 2);
  Tree other;
  other.insert("key1", 1);
  other.insert("key2", 2);

  // Assert
  ASSERT_EQ(tree_, other);
}

TEST_F(RBTreeTest, InequalityOperator) {
  // Arrange
  tree_.insert("key1", 1);
  Tree other;
  other.insert("key1", 2);  // Same key, different value

  // Assert
  ASSERT_NE(tree_, other);
}

// Edge Cases
TEST_F(RBTreeTest, SuccessorOfMaxReturnsNull) {
  // Arrange
  tree_.insert("key1", 1);
  Node* max_node = tree_.max();

  // Act
  Node* succ = tree_.successor(max_node);

  // Assert
  ASSERT_EQ(succ, nullptr);
}

TEST_F(RBTreeTest, PredecessorOfMinReturnsNull) {
  // Arrange
  tree_.insert("key1", 1);
  Node* min_node = tree_.min();

  // Act
  Node* pred = tree_.predecessor(min_node);

  // Assert
  ASSERT_EQ(pred, nullptr);
}

TEST_F(RBTreeTest, EraseNonExistentDoesNothing) {
  // Arrange
  tree_.insert("key1", 1);
  size_t initial_size = tree_.size();

  // Act
  tree_.erase("nonexistent");

  // Assert
  ASSERT_EQ(tree_.size(), initial_size);
  validate_tree_properties(tree_.root());
}
