test_suite(
    name = "utils_tests",
    tests = [
        ":parse_test",
        ":to_string_test",
    ],
)

cc_test(
    name = "to_string_test",
    srcs = ["to_string_test.cc"],
    deps = [
        "//src/utils:parse",
        "//src/utils:to_string",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "parse_test",
    srcs = ["parse_test.cc"],
    deps = [
        "//src/utils:parse",
        "@googletest//:gtest_main",
    ],
)
