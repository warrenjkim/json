#pragma once

#include <string>

#include "node.h"

namespace json {

namespace nodes {

class String : public Node {
 public:
  Node* clone() const override;

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
