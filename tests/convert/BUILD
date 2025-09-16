cc_test(
    name = "lower_test",
    srcs = ["lower_test.cc"],
    deps = [
        "//src/api",
        "//src/ast",
        "//src/convert",
        "@googletest//:gtest_main",
    ],
)

test_suite(
    name = "convert_tests",
    tests = [
        ":lower_test",
    ],
)
