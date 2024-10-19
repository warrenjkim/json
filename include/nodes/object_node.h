#pragma once

#include <cstddef>
#include <string>

#include "node.h"
#include "utils/rbt.h"

namespace json {

class ObjectNode : public Node {
 public:
  ~ObjectNode();

 public:
  void accept(Visitor& visitor) const override;

 public:
  void add(const std::string& key, Node* value);
  const utils::Map<std::string, Node*>& get() const;
  const size_t size() const;
  const bool empty() const;

 private:
  utils::Map<std::string, Node*> properties_;

 public:
  ObjectNode() = default;
};

}  // namespace json
