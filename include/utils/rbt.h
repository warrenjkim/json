#pragma once

#include <cstddef>
#include <string>

namespace json {

namespace utils {

namespace rbt {

enum class Color { RED, BLACK };
enum class Structure { LEFT_LEFT, RIGHT_RIGHT, LEFT_RIGHT, RIGHT_LEFT };

}  // namespace rbt

template <typename T>
class RBTree {
 public:
  struct Node {
    std::string key;
    T* data;
    Node* left;
    Node* right;

    rbt::Color color;
    Node* parent;

    Node(std::string key, T* data)
        : key(std::move(key)),
          data(data),
          color(rbt::Color::RED),
          left(nullptr),
          right(nullptr),
          parent(nullptr) {}

    ~Node() = default;
  };

 public:
  RBTree() : root_(nullptr), size_(0) {}

  ~RBTree() {
    clear(root_);
    root_ = nullptr;
    size_ = 0;
  }

 public:
  const Node* root() const { return root_; }

  const size_t size() const { return size_; }

 public:
  void insert(const std::string& key, T* data) {
    Node* z = nullptr;
    size_t size = size_;
    root_ = recursive_insert(root_, key, data, z);
    if (size_ == size) {
      return;
    }

    root_ = double_red_fixup(z);
  }

  void remove(const std::string& target) {
    Node* del = find(target);
    if (!del) {
      return;
    }

    root_ = recursive_remove(del);
    size_--;
  }

  Node* find(const std::string& target) const {
    return recursive_find(root_, target);
  }

  void clear() {
    clear(root_);
    root_ = nullptr;
    size_ = 0;
  }

 private:
  Node* recursive_insert(Node* root, const std::string& key, T* data,
                         Node*& z) {
    if (!root) {
      size_++;
      z = new Node(key, data);
      return z;
    }

    if (key < root->key) {
      root->left = recursive_insert(root->left, key, data, z);
      if (root->left == z) {
        z->parent = root;
      }
    } else if (key > root->key) {
      root->right = recursive_insert(root->right, key, data, z);
      if (root->right == z) {
        z->parent = root;
      }
    } else {
      delete root->data;
      root->data = data;
    }

    return root;
  }

  Node* recursive_remove(Node* target) {
    if (target->left && target->right) {
      Node* replacement = successor(target);
      std::swap(target->key, replacement->key);
      std::swap(target->data, replacement->data);
      return recursive_remove(replacement);
    }

    Node* replacement = target->left ? target->left : target->right;
    Node* parent = target->parent;
    rbt::Color target_color = target->color;

    // relink parent to point to replacement
    if (parent) {
      if (parent->left == target) {
        parent->left = replacement;
      } else {
        parent->right = replacement;
      }
    } else {
      root_ = replacement;
    }

    // update replacement to point to parent
    if (replacement) {
      replacement->parent = parent;
    }

    delete target->data;
    delete target;

    if (target_color == rbt::Color::BLACK) {
      if (replacement && replacement->color == rbt::Color::RED) {
        replacement->color = rbt::Color::BLACK;
      } else {
        Node* z = double_black_fixup(replacement, parent);
        while (z && z->parent) {
          z = z->parent;
        }

        return z;
      }
    }

    return root_;
  }

  Node* recursive_find(Node* root, const std::string& target) const {
    if (!root) {
      return nullptr;
    }

    if (target < root->key) {
      return recursive_find(root->left, target);
    } else if (target > root->key) {
      return recursive_find(root->right, target);
    }

    return root;
  }

  void clear(Node* node) {
    if (node == nullptr) {
      return;
    }
    clear(node->left);
    clear(node->right);
    delete node->data;
    delete node;
  }

  Node* successor(Node* node) {
    Node* successor = node->right;
    while (successor->left) {
      successor = successor->left;
    }

    return successor;
  }

 private:
  Node* double_red_fixup(Node* z) {
    if (!z->parent) {
      z->color = rbt::Color::BLACK;
      return z;
    }

    if (is_black(z) || is_black(z->parent)) {
      return double_red_fixup(z->parent);
    }

    Node* u = uncle(z);
    if (is_black(u)) {
      z = restructure(z);
      z->color = rbt::Color::BLACK;
      z->left->color = rbt::Color::RED;
      z->right->color = rbt::Color::RED;
      return double_red_fixup(z);
    }

    return double_red_fixup(recolor(z));
  }

  Node* double_black_fixup(Node* replacement, Node* parent) {
    if (!parent) {
      return replacement;
    }

    Node* sibling =
        (parent->left == replacement) ? parent->right : parent->left;

    // case 1: red sibling
    if (is_red(sibling)) {
      Node* z = (parent->left == sibling) ? sibling->left : sibling->right;
      z = restructure(z);
      z->color = rbt::Color::BLACK;
      parent->color = rbt::Color::RED;
      return double_black_fixup(replacement, parent);
    }

    // case 2: black sibling with two black children
    if (is_black(sibling->left) && is_black(sibling->right)) {
      sibling->color = rbt::Color::RED;
      if (is_red(parent)) {
        parent->color = rbt::Color::BLACK;

        return parent;
      } else {
        return double_black_fixup(parent, parent->parent);
      }
    }

    // case 3: black sibling with at least one red child
    Node* z = nullptr;
    if (is_red(sibling->left) && is_red(sibling->right)) {
      z = (parent->left == sibling) ? sibling->left : sibling->right;
    } else {
      z = is_red(sibling->left) ? sibling->left : sibling->right;
    }

    z = restructure(z);
    z->color = parent->color;
    z->left->color = rbt::Color::BLACK;
    z->right->color = rbt::Color::BLACK;

    return z;
  }

  Node* recolor(Node* z) {
    z->parent->color = rbt::Color::BLACK;
    uncle(z)->color = rbt::Color::BLACK;
    grandparent(z)->color = rbt::Color::RED;

    return grandparent(z);
  }

  Node* restructure(Node* z) {
    rbt::Structure structure;
    if (grandparent(z)->left == z->parent) {
      structure = z->parent->left == z ? rbt::Structure::LEFT_LEFT
                                       : rbt::Structure::LEFT_RIGHT;
    } else {
      structure = z->parent->left == z ? rbt::Structure::RIGHT_LEFT
                                       : rbt::Structure::RIGHT_RIGHT;
    }

    switch (structure) {
      case rbt::Structure::LEFT_LEFT:
        z = right_rotate(grandparent(z));
        break;
      case rbt::Structure::LEFT_RIGHT:
        z = right_rotate(left_rotate(z->parent)->parent);
        break;
      case rbt::Structure::RIGHT_LEFT:
        z = left_rotate(right_rotate(z->parent)->parent);
        break;
      case rbt::Structure::RIGHT_RIGHT:
        z = left_rotate(grandparent(z));
        break;
    }

    return z;
  }

 private:
  Node* left_rotate(Node* node) {
    Node* new_root = node->right;
    node->right = new_root->left;
    if (new_root->left) {
      new_root->left->parent = node;
    }

    new_root->parent = node->parent;
    if (node->parent) {
      if (node->parent->left == node) {
        node->parent->left = new_root;
      } else {
        node->parent->right = new_root;
      }
    }

    new_root->left = node;
    node->parent = new_root;

    return new_root;
  }

  Node* right_rotate(Node* node) {
    Node* new_root = node->left;

    node->left = new_root->right;
    if (new_root->right) {
      new_root->right->parent = node;
    }

    new_root->parent = node->parent;
    if (node->parent) {
      if (node->parent->left == node) {
        node->parent->left = new_root;
      } else {
        node->parent->right = new_root;
      }
    }

    new_root->right = node;
    node->parent = new_root;

    return new_root;
  }

 private:
  Node* grandparent(Node* node) { return node->parent->parent; }

  Node* uncle(Node* z) {
    Node* gp = grandparent(z);
    return gp->left == z->parent ? gp->right : gp->left;
  }

 private:
  bool is_black(const Node* node) const {
    return !node || node->color == rbt::Color::BLACK;
  }

  bool is_red(const Node* node) const {
    return node && node->color == rbt::Color::RED;
  }

 private:
  Node* root_;
  size_t size_;
};

}  // namespace utils

}  // namespace json
