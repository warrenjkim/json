#include "utils/rbt.h"

#include <algorithm>

#include "nodes/node.h"

namespace json {

namespace utils {

RBTree::RBTree() : root_(nullptr), size_(0) {}

RBTree::~RBTree() {
  clear(root_);
  root_ = nullptr;
  size_ = 0;
}

const RBTree::Node* RBTree::root() const { return root_; }

const size_t RBTree::size() const { return size_; }

void RBTree::insert(const std::string& key, json::Node* data) {
  RBTree::Node* z = nullptr;
  size_t size = size_;
  root_ = recursive_insert(root_, key, data, z);
  if (size_ == size) {
    return;
  }

  root_ = double_red_fixup(z);
}

void RBTree::remove(const std::string& target) {
  RBTree::Node* del = find(target);
  if (!del) {
    return;
  }

  root_ = recursive_remove(del);
  size_--;
}

RBTree::Node* RBTree::find(const std::string& target) const {
  return recursive_find(root_, target);
}

void RBTree::clear() {
  clear(root_);
  root_ = nullptr;
  size_ = 0;
}

RBTree::Node* RBTree::recursive_insert(RBTree::Node* root,
                                       const std::string& key, json::Node* data,
                                       RBTree::Node*& z) {
  if (!root) {
    size_++;
    z = new RBTree::Node(key, data);
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

RBTree::Node* RBTree::recursive_remove(RBTree::Node* target) {
  if (target->left && target->right) {
    RBTree::Node* replacement = successor(target);
    std::swap(target->key, replacement->key);
    std::swap(target->data, replacement->data);
    return recursive_remove(replacement);
  }

  RBTree::Node* replacement = target->left ? target->left : target->right;
  RBTree::Node* parent = target->parent;
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
      RBTree::Node* z = double_black_fixup(replacement, parent);
      while (z && z->parent) {
        z = z->parent;
      }

      return z;
    }
  }

  return root_;
}

RBTree::Node* RBTree::recursive_find(RBTree::Node* root,
                                     const std::string& target) const {
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

void RBTree::clear(Node* node) {
  if (node == nullptr) {
    return;
  }
  clear(node->left);
  clear(node->right);
  delete node->data;
  delete node;
}

RBTree::Node* RBTree::successor(RBTree::Node* node) {
  RBTree::Node* successor = node->right;
  while (successor->left) {
    successor = successor->left;
  }

  return successor;
}

RBTree::Node* RBTree::double_red_fixup(RBTree::Node* z) {
  if (!z->parent) {
    z->color = rbt::Color::BLACK;
    return z;
  }

  if (is_black(z) || is_black(z->parent)) {
    return double_red_fixup(z->parent);
  }

  RBTree::Node* u = uncle(z);
  if (is_black(u)) {
    z = restructure(z);
    z->color = rbt::Color::BLACK;
    z->left->color = rbt::Color::RED;
    z->right->color = rbt::Color::RED;
    return double_red_fixup(z);
  }

  return double_red_fixup(recolor(z));
}

RBTree::Node* RBTree::double_black_fixup(RBTree::Node* replacement,
                                         RBTree::Node* parent) {
  if (!parent) {
    return replacement;
  }

  RBTree::Node* sibling =
      (parent->left == replacement) ? parent->right : parent->left;

  // case 1: red sibling
  if (is_red(sibling)) {
    RBTree::Node* z =
        (parent->left == sibling) ? sibling->left : sibling->right;
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
  RBTree::Node* z = nullptr;
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

RBTree::Node* RBTree::recolor(RBTree::Node* z) {
  z->parent->color = rbt::Color::BLACK;
  uncle(z)->color = rbt::Color::BLACK;
  grandparent(z)->color = rbt::Color::RED;

  return grandparent(z);
}

RBTree::Node* RBTree::restructure(RBTree::Node* z) {
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

RBTree::Node* RBTree::left_rotate(RBTree::Node* node) {
  RBTree::Node* new_root = node->right;
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

RBTree::Node* RBTree::right_rotate(RBTree::Node* node) {
  RBTree::Node* new_root = node->left;

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

RBTree::Node* RBTree::grandparent(RBTree::Node* node) {
  return node->parent->parent;
}

RBTree::Node* RBTree::uncle(RBTree::Node* z) {
  RBTree::Node* gp = grandparent(z);
  return gp->left == z->parent ? gp->right : gp->left;
}

bool RBTree::is_black(const RBTree::Node* node) const {
  return !node || node->color == rbt::Color::BLACK;
}

bool RBTree::is_red(const RBTree::Node* node) const {
  return node && node->color == rbt::Color::RED;
}

}  // namespace utils

}  // namespace json
