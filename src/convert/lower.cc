#include "lower.h"

#include <map>
#include <utility>
#include <vector>

#include "warren/json/internal/ast/array.h"
#include "warren/json/internal/ast/boolean.h"
#include "warren/json/internal/ast/node.h"
#include "warren/json/internal/ast/null.h"
#include "warren/json/internal/ast/number.h"
#include "warren/json/internal/ast/object.h"
#include "warren/json/internal/ast/string.h"
#include "warren/json/internal/ast/visitor.h"
#include "warren/json/value.h"

namespace {

class LowerVisitor : public json::ast::Visitor {
 public:
  void visit(const json::ast::Array& node) override {
    std::vector<json::Value> values;
    for (const json::ast::Node* value : node.value) {
      json::Value v = std::move(value_);
      value->accept(*this);
      std::swap(v, value_);
      values.push_back(std::move(v));
    }

    value_ = std::move(values);
  }

  void visit(const json::ast::Boolean& node) override { value_ = node.value; }

  void visit(const json::ast::Null&) override { value_ = nullptr; }

  void visit(const json::ast::Number& node) override {
    switch (node.type) {
      case json::ast::Number::Type::DOUBLE:
        value_ = node.dbl;
        break;
      case json::ast::Number::Type::INTEGRAL:
        value_ = node.intgr;
        break;
    };
  }

  void visit(const json::ast::Object& node) override {
    std::map<std::string, json::Value> values;
    for (const auto& [key, value] : node.value) {
      json::Value v = std::move(value_);
      value->accept(*this);
      std::swap(v, value_);
      values[key] = std::move(v);
    }

    value_ = std::move(values);
  }

  void visit(const json::ast::String& node) override { value_ = node.value; }

  json::Value value() const { return value_; }

 private:
  json::Value value_;
};

}  // namespace

namespace json {
namespace convert {

Value lower(const ast::Node& node) {
  LowerVisitor visitor;
  node.accept(visitor);

  return visitor.value();
}

}  // namespace convert
}  // namespace json
