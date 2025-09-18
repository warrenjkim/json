test_suite(
    name = "convert_tests",
    tests = [
        ":lower_test",
    ],
)

cc_test(
    name = "lower_test",
    srcs = ["lower_test.cc"],
    deps = [
        "//src/api:value",
        "//src/ast:nodes",
        "//src/ast:visitor",
        "//src/convert:lower",
        "//src/utils:exception",
        "@googletest//:gtest_main",
    ],
)
