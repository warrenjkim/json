#pragma once

#include <optional>
#include <string>
#include <string_view>

#include "warren/internal/nodes/node.h"
#include "warren/internal/visitors/visitor.h"

namespace json {

namespace visitors {

class GetVisitor : public Visitor {
 public:
  GetVisitor();
  GetVisitor(const std::string_view keys);
  GetVisitor(const size_t keys);

 public:
  void visit(nodes::Array& node) override;
  void visit(nodes::Boolean& node) override;
  void visit(nodes::Null& node) override;
  void visit(nodes::Number& node) override;
  void visit(nodes::Object& node) override;
  void visit(nodes::String& node) override;

 public:
  nodes::Node* result();

 public:
  ~GetVisitor() = default;

 private:
  nodes::Node* ast_;
  std::optional<std::string> key_;
  std::optional<size_t> index_;
  nodes::Node* result_;
};

}  // namespace visitors

}  // namespace json
