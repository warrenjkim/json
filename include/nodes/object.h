#pragma once

#include <cstddef>
#include <string>

#include "node.h"
#include "utils/map.h"

namespace json {

class Object : public Node {
 public:
  void accept(visitors::Visitor& visitor) override;
  void accept(visitors::ConstVisitor& visitor) const override;

 public:
  Node* clone() override;

 public:
  Object() = default;
  Object(const utils::Map<std::string, Node*>& properties);
  ~Object();

  Object(Object&&) = delete;
  Object(const Object&) = delete;
  Object& operator=(Object&&) = delete;
  Object& operator=(const Object&) = delete;

 public:
  void add(const std::string& key, Node* value);
  void put(const std::string& key, Node* value);
  const size_t size() const;
  const bool empty() const;

 public:
  utils::Map<std::string, Node*>& get();
  const utils::Map<std::string, Node*>& get() const;

 private:
  utils::Map<std::string, Node*> properties_;
};

}  // namespace json
