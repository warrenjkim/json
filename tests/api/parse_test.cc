#include "warren/json/parse.h"

#include "gtest/gtest.h"
#include "warren/json/value.h"

TEST(ParseTest, Parse) {
  EXPECT_EQ(R"(
  {
    "key": "value",
    "other": 10
  }
      )"_json,
            json::Value(json::parse("{\"key\": \"value\", \"other\": 10}")));
}
