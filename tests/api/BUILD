cc_test(
    name = "value_test",
    srcs = ["value_test.cc"],
    deps = [
        "//src/api",
        "//src/utils",
        "@googletest//:gtest_main",
    ],
)

test_suite(
    name = "api_tests",
    tests = [
        ":value_test",
    ],
)
