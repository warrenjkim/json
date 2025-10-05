test_suite(
    name = "parse_tests",
    tests = [
        ":lexer_test",
        ":parser_test",
    ],
)

cc_test(
    name = "lexer_test",
    srcs = ["lexer_test.cc"],
    deps = [
        "//src/parse:lexer",
        "//src/parse:token",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "parser_test",
    srcs = ["parser_test.cc"],
    deps = [
        "//src/parse:lexer",
        "//src/parse:parser",
        "//src/parse:token",
        "//src/utils:exception",
        "@googletest//:gtest_main",
    ],
)
