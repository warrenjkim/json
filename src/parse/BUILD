package(default_visibility = ["//src:__subpackages__"])

cc_library(
    name = "parse",
    srcs = [
        "lexer.cc",
        "parser.cc",
    ],
    hdrs = [
        "lexer.h",
        "parser.h",
        "token.h",
    ],
    include_prefix = "warren/json/internal/parse",
    strip_include_prefix = ".",
    visibility = [
        "//src:__subpackages__",
        "//tests:__subpackages__",
    ],
    deps = [
        "//src/ast",
        "//src/utils",
    ],
)
