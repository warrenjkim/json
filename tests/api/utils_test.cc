#include "warren/json/utils.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "warren/json/to_string.h"

namespace warren {
namespace json {

namespace {

using ::testing::Eq;

TEST(UtilsTest, Parse) {
  EXPECT_THAT(R"(
  {
    "key": "value",
    "other": 10
  }
      )"_json,
              Eq(parse("{\"key\": \"value\", \"other\": 10}")));
}

TEST(UtilsTest, PrettyPrint) {
  Value value = R"json({
    "meta": null,
    "active": true,
    "count": 42,
    "ratio": 0.125,
    "message": "All systems nominal",
    "data": {
      "values": [1, 2.5, null, false, "ok"],
      "details": {
        "thresholds": { "min": -1, "max": 10 },
        "empty_array": [],
        "empty_object": {}
      }
    },
    "logs": [
      { "level": "info", "text": "startup complete" },
      { "level": "warn", "text": "low memory" }
    ]
  })json"_json;

  EXPECT_THAT(to_string(value), Eq(std::string(R"({
  "active": true,
  "count": 42,
  "data": {
    "details": {
      "empty_array": [],
      "empty_object": {},
      "thresholds": {
        "max": 10,
        "min": -1
      }
    },
    "values": [
      1,
      2.5,
      null,
      false,
      "ok"
    ]
  },
  "logs": [
    {
      "level": "info",
      "text": "startup complete"
    },
    {
      "level": "warn",
      "text": "low memory"
    }
  ],
  "message": "All systems nominal",
  "meta": null,
  "ratio": 0.125
})")));
}

TEST(UtilsTest, PrettyPrintFourSpaceTabs) {
  Value value = R"json({
    "meta": null,
    "active": true,
    "count": 42,
    "ratio": 0.125,
    "message": "All systems nominal",
    "data": {
      "values": [1, 2.5, null, false, "ok"],
      "details": {
        "thresholds": { "min": -1, "max": 10 },
        "empty_array": [],
        "empty_object": {}
      }
    },
    "logs": [
      { "level": "info", "text": "startup complete" },
      { "level": "warn", "text": "low memory" }
    ]
  })json"_json;

  EXPECT_THAT(to_string(value, PrintOptions{.tab_width = 4}),
              Eq(std::string(R"({
    "active": true,
    "count": 42,
    "data": {
        "details": {
            "empty_array": [],
            "empty_object": {},
            "thresholds": {
                "max": 10,
                "min": -1
            }
        },
        "values": [
            1,
            2.5,
            null,
            false,
            "ok"
        ]
    },
    "logs": [
        {
            "level": "info",
            "text": "startup complete"
        },
        {
            "level": "warn",
            "text": "low memory"
        }
    ],
    "message": "All systems nominal",
    "meta": null,
    "ratio": 0.125
})")));
}

}  // namespace

}  // namespace json
}  // namespace warren
