cc_test(
    name = "lexer_test",
    srcs = ["lexer_test.cc"],
    deps = [
        "//src/api",
        "//src/ast",
        "//src/parse",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "parser_test",
    srcs = ["parser_test.cc"],
    deps = [
        "//src/api",
        "//src/ast",
        "//src/parse",
        "@googletest//:gtest_main",
    ],
)

test_suite(
    name = "parse_tests",
    tests = [
        ":lexer_test",
        ":parser_test",
    ],
)
