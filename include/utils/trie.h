#pragma once

#include <cstddef>
#include <string>
#include <string_view>
#include <unordered_map>

#include "macros.h"

namespace json {

namespace utils {

template <typename T>
class Trie {
 public:
  struct Node {
    std::unordered_map<char, Node*> children;
    T* ref;
    bool end;

    Node() : ref(nullptr), end(false) {}

    explicit Node(T* ref) : ref(ref), end(false) {}

    ~Node() {
      for (auto& [_, child] : children) {
        delete child;
      }
    }
  };

 public:
  Trie() : root_(nullptr), size_(0) {}

  ~Trie() { clear(); }

  Trie(Trie&& other) noexcept : root_(other.root_), size_(other.size_) {
    other.root_ = nullptr;
    other.size_ = 0;
  }

  Trie& operator=(Trie&& other) noexcept {
    if (this != &other) {
      clear();

      root_ = other.root_;
      size_ = other.size_;

      other.root_ = nullptr;
      other.size_ = 0;
    }
    return *this;
  }

 public:
  const Node* root() const { return root_; }

  const size_t size() const { return size_; }

  T* get(const std::string_view key) {
    if (!root_) {
      return nullptr;
    }

    Node* curr = root_;
    for (const char c : key) {
      if (curr->children.find(c) == curr->children.end()) {
        return nullptr;
      }

      curr = curr->children[c];
    }

    if (curr->end) {
      return curr->ref;
    }

    return nullptr;
  }

 public:
  void clear() {
    if (!root_) {
      return;
    }

    delete root_;
    root_ = nullptr;
    size_ = 0;
  }

  void insert(const std::string_view key, T* ref) {
    if (!root_) {
      root_ = new Node(ref);
    }

    Node* curr = root_;
    for (const char c : key) {
      if (curr->children.find(c) == curr->children.end()) {
        curr->children[c] = new Node(ref);
      }

      curr = curr->children[c];
    }

    curr->end = true;
    size_++;
  }

  bool contains(const std::string_view target) const {
    if (!root_) {
      return false;
    }

    Node* curr = root_;
    for (const char c : target) {
      if (curr->children.find(c) == curr->children.end()) {
        return false;
      }

      curr = curr->children[c];
    }

    return curr->end;
  }

  bool contains_prefix(const std::string_view target) const {
    if (!root_) {
      return false;
    }

    Node* curr = root_;
    for (const char c : target) {
      if (curr->children.find(c) == curr->children.end()) {
        return false;
      }

      curr = curr->children[c];
    }

    return true;
  }

  void remove(const std::string_view target) {
    if (!contains(target)) {
      return;
    }

    root_ = remove(root_, target, 0);
    size_--;
  }

  std::string visualize_keys() const {
    std::string result = visualize_keys(root_, /* prefix= */ "",
                                        /* last_child= */ false);
    DEBUG("trie", "\n" << result, /* indent_level= */ 0);

    return result;
  }

 private:
  Node* remove(Node* root, const std::string_view target,
               const size_t index = 0) {
    if (index == target.length()) {
      root->end = false;
      if (root->children.empty()) {
        delete root;
        root = nullptr;
      }

      return root;
    }

    char c = target[index];
    root->children[c] = remove(root->children[c], target, index + 1);

    if (!root->children[c]) {
      root->children.erase(c);
    }

    if (root->children.empty() && !root->end) {
      delete root;
      root = nullptr;
    }

    return root;
  }

  std::string visualize_keys(const Node* root, const std::string& prefix,
                             const bool last_child) const {
    std::string result = "";
    for (auto it = root->children.begin(); it != root->children.end(); it++) {
      result += prefix;
      char c = last_child ? ' ' : '|';
      result += c;
      result += "   ";
      result += "|-- ";
      result += it->first;
      if (it->second->end) {
        result += " (end)";
      }

      result +=
          "\n" + visualize_keys(
                     /* root= */ it->second,
                     /* prefix= */ prefix + (last_child ? "    " : "|   "),
                     /* last_child= */ std::next(it) == root->children.end());
    }

    return result;
  }

 private:
  Node* root_;
  size_t size_;

 private:
  Trie(const Trie&) = delete;
  Trie& operator=(const Trie&) = delete;
};

}  // namespace utils

}  // namespace json
