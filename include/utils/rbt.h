#pragma once

#include <cstddef>
#include <string>

#include "nodes/node.h"

namespace json {

namespace utils {

namespace rbt {

enum class Color { RED, BLACK };
enum class Structure { LEFT_LEFT, RIGHT_RIGHT, LEFT_RIGHT, RIGHT_LEFT };

}  // namespace rbt

class RBTree {
 public:
  struct Node {
    std::string key;
    json::Node* data;
    Node* left;
    Node* right;

    rbt::Color color;
    Node* parent;

    Node(std::string key, json::Node* data)
        : key(std::move(key)),
          data(data),
          color(rbt::Color::RED),
          left(nullptr),
          right(nullptr),
          parent(nullptr) {}

    ~Node() = default;
  };

 public:
  RBTree();
  ~RBTree();

 public:
  const Node* root() const;
  const size_t size() const;

 public:
  void insert(const std::string& key, json::Node* data);
  void remove(const std::string& target);
  Node* find(const std::string& target) const;
  void clear();

 private:
  Node* recursive_insert(Node* root, const std::string& key, json::Node* data,
                         Node*& z);
  Node* recursive_remove(Node* target);
  Node* recursive_find(Node* root, const std::string& target) const;
  void clear(Node* node);
  Node* successor(Node* node);

 private:
  Node* double_red_fixup(Node* z);
  Node* double_black_fixup(Node* target, Node* replacement);
  Node* recolor(Node* node);
  Node* restructure(Node* node);

 private:
  Node* left_rotate(Node* root);
  Node* right_rotate(Node* root);

 private:
  Node* grandparent(Node* z);
  Node* uncle(Node* z);
  Node* sibling(Node* node);

 private:
  bool is_black(const Node* node) const;
  bool is_red(const Node* node) const;

 private:
  Node* root_;
  size_t size_;
};

}  // namespace utils

}  // namespace json
