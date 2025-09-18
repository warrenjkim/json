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
  root.value.push_back(new json::ast::Array(
      {new json::ast::Null(), new json::ast::Number(1),
       new json::ast::Boolean(true), new json::ast::String("string")}));
  root.value.push_back(
      new json::ast::Object({{"null", new json::ast::Null()},
                             {"number", new json::ast::Number(1)},
                             {"boolean", new json::ast::Boolean(true)},
                             {"string", new json::ast::String("string")}}));

  json::Value value = json::convert::lower(root);

  EXPECT_EQ(value,
            json::Value(json::array_t{nullptr, 1, true, "string",
                                      json::array_t{nullptr, 1, true, "string"},
                                      json::object_t{{"null", nullptr},
                                                     {"number", 1},
                                                     {"boolean", true},
                                                     {"string", "string"}}}));
}

TEST(LowerTest, Object) {
  json::ast::Object root({});
  root.value["null"] = new json::ast::Null();
  root.value["number"] = new json::ast::Number(1);
  root.value["boolean"] = new json::ast::Boolean(true);
  root.value["string"] = new json::ast::String("string");
  root.value["array"] = new json::ast::Array(
      {new json::ast::Null(), new json::ast::Number(1),
       new json::ast::Boolean(true), new json::ast::String("string")});
  root.value["object"] =
      new json::ast::Object({{"null", new json::ast::Null()},
                             {"number", new json::ast::Number(1)},
                             {"boolean", new json::ast::Boolean(true)},
                             {"string", new json::ast::String("string")}});

  json::Value value = json::convert::lower(root);

  EXPECT_EQ(value, json::Value({
                       {"null", nullptr},
                       {"number", 1},
                       {"boolean", true},
                       {"string", "string"},
                       {"array", json::array_t{nullptr, 1, true, "string"}},
                       {"object", json::object_t{{"null", nullptr},
                                                 {"number", 1},
                                                 {"boolean", true},
                                                 {"string", "string"}}},
                   }));
}
