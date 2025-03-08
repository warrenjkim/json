#pragma once

#include <string>

#include "node.h"

namespace json {

namespace nodes {

class String : public Node {
 public:
  void accept(visitors::Visitor& visitor) override;
  void accept(visitors::ConstVisitor& visitor) const override;

 public:
  Node* clone() override;

 public:
  String(std::string value);

 public:
  std::string& get();
  const std::string& get() const;

 private:
  std::string value_;

 public:
  String() = delete;
};

}  // namespace nodes

}  // namespace json
