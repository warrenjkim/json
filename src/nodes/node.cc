#include "nodes/node.h"

#include "visitors/cmp_visitor.h"

namespace json {

namespace nodes {

const bool Node::operator==(const Node& other) const {
  visitors::CmpVisitor cmp_visitor(this);
  other.accept(cmp_visitor);

  return cmp_visitor.result();
}

const bool Node::operator!=(const Node& other) const {
  return !(*this == other);
}

}  // namespace nodes

}  // namespace json
