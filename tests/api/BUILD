test_suite(
    name = "api_tests",
    tests = [
        ":parse_test",
        ":value_test",
    ],
)

cc_test(
    name = "value_test",
    srcs = ["value_test.cc"],
    deps = [
        "//src/api:value",
        "//src/utils:exception",
        "//src/utils:types",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "parse_test",
    srcs = ["parse_test.cc"],
    deps = [
        "//src/api:parse",
        "@googletest//:gtest_main",
    ],
)
