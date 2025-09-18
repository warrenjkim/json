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

test_suite(
    name = "api_tests",
    tests = [
        ":value_test",
    ],
)
