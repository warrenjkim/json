#pragma once

#include <cstddef>
#include <map>
#include <string>

#include "node.h"

namespace json {

namespace nodes {

class Object : public Node {
 public:
  Node* clone() const override;

 public:
  Object() = default;
  Object(const std::map<std::string, Node*>& properties);
  ~Object();

  Object(Object&&) = delete;
  Object(const Object&) = delete;
  Object& operator=(Object&&) = delete;
  Object& operator=(const Object&) = delete;

 public:
  void insert(const std::string& key, Node* value);
  const size_t size() const;
  const bool empty() const;

 public:
  std::map<std::string, Node*>& get();
  const std::map<std::string, Node*>& get() const;

 private:
  std::map<std::string, Node*> properties_;
};

}  // namespace nodes

}  // namespace json
