#pragma once

#include <cstddef>
#include <string>

#include "node.h"
#include "warren/internal/dsa/map.h"

namespace json {

namespace nodes {

class Object : public Node {
 public:
  void accept(visitors::Visitor& visitor) override;
  void accept(visitors::ConstVisitor& visitor) const override;

 public:
  Node* clone() override;

 public:
  Object() = default;
  Object(const dsa::Map<std::string, Node*>& properties);
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
  dsa::Map<std::string, Node*>& get();
  const dsa::Map<std::string, Node*>& get() const;

 private:
  dsa::Map<std::string, Node*> properties_;
};

}  // namespace nodes

}  // namespace json
