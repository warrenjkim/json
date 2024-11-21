#pragma once

#include <cstddef>
#include <string>

#include "node.h"
#include "utils/rbt.h"

namespace json {

class Object : public Node {
 public:
  void accept(visitors::Visitor& visitor) override;
  void accept(visitors::ConstVisitor& visitor) const override;

 public:
  Node* clone() override;

 public:
  ~Object();

 public:
  void add(const std::string& key, Node* value);
  const size_t size() const;
  const bool empty() const;

 public:
  utils::Map<std::string, Node*>& get();
  const utils::Map<std::string, Node*>& get() const;

 public:
  utils::Map<std::string, Node*>* ptr();
  const utils::Map<std::string, Node*>* ptr() const;

 private:
  utils::Map<std::string, Node*> properties_;
};

}  // namespace json
