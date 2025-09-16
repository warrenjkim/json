#include "warren/json/internal/convert/lower.h"

#include <map>
#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "warren/json/internal/ast/array.h"
#include "warren/json/internal/ast/boolean.h"
#include "warren/json/internal/ast/null.h"
#include "warren/json/internal/ast/number.h"
#include "warren/json/internal/ast/object.h"
#include "warren/json/internal/ast/string.h"
#include "warren/json/value.h"

TEST(LowerTest, Array) {
  json::ast::Array root({});
  root.value.push_back(new json::ast::Null());
  root.value.push_back(new json::ast::Number(1));
  root.value.push_back(new json::ast::Boolean(true));
  root.value.push_back(new json::ast::String("string"));
  json::Value value = json::convert::lower(root);

  std::vector<json::Value> expected = {nullptr, 1, true, "string"};
  EXPECT_EQ(value, json::Value(std::move(expected)));
}

TEST(LowerTest, Object) {
  json::ast::Object root({});
  root.value["null"] = new json::ast::Null();
  root.value["number"] = new json::ast::Number(1);
  root.value["boolean"] = new json::ast::Boolean(true);
  root.value["string"] = new json::ast::String("string");
  json::Value value = json::convert::lower(root);

  std::map<std::string, json::Value> expected = {{"null", nullptr},
                                                 {"number", 1},
                                                 {"boolean", true},
                                                 {"string", "string"}};
  EXPECT_EQ(value, json::Value(std::move(expected)));
}
