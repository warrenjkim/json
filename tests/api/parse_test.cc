#include "warren/json/parse.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace {

using ::testing::Eq;

TEST(ParseTest, Parse) {
  EXPECT_THAT(R"(
  {
    "key": "value",
    "other": 10
  }
      )"_json,
              Eq(json::parse("{\"key\": \"value\", \"other\": 10}")));
}

}  // namespace
