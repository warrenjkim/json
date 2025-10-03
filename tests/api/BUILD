test_suite(
    name = "api_tests",
    tests = [
        ":utils_test",
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
    name = "utils_test",
    srcs = ["utils_test.cc"],
    deps = [
        "//src/api:utils",
        "@googletest//:gtest_main",
    ],
)
