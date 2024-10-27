#pragma once

#include <cstddef>
#include <string>

#include "node.h"
#include "utils/rbt.h"

namespace json {

class Object : public Node {
 public:
  void accept(visitors::Visitor& visitor) const override;
  Node* accept(visitors::ReturnVisitor& visitor) const override;

 public:
  void add(const std::string& key, Node* value);
  const size_t size() const;
  const bool empty() const;

 public:
  utils::Map<std::string, Node*>& get();
  const utils::Map<std::string, Node*>& get() const;

 private:
  utils::Map<std::string, Node*> properties_;
};

}  // namespace json
