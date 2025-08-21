#pragma once

namespace json {

namespace nodes {

class Node {
 public:
  virtual Node* clone() const = 0;

 public:
  virtual ~Node() noexcept = default;
};

}  // namespace nodes

}  // namespace json
