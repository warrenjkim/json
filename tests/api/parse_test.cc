#include "warren/json/parse.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace warren {
namespace json {

namespace {

using ::testing::Eq;

TEST(ParseTest, Parse) {
  EXPECT_THAT(R"(
  {
    "key": "value",
    "other": 10
  }
      )"_json,
              Eq(parse("{\"key\": \"value\", \"other\": 10}")));
}

}  // namespace

}  // namespace json
}  // namespace warren
