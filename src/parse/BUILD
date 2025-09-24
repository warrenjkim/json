cc_library(
    name = "parse",
    include_prefix = "warren/json/internal/parse",
    strip_include_prefix = ".",
    visibility = [
        "//src:__subpackages__",
        "//tests:__subpackages__",
    ],
    deps = [
        ":lexer",
        ":parser",
        ":token",
    ],
)

cc_library(
    name = "parser",
    srcs = [
        "parser.cc",
    ],
    hdrs = [
        "parser.h",
    ],
    include_prefix = "warren/json/internal/parse",
    strip_include_prefix = ".",
    visibility = [
        "//src:__subpackages__",
        "//tests:__subpackages__",
    ],
    deps = [
        ":lexer",
        ":token",
        "//src/api:value",
        "//src/utils:exception",
    ],
)

cc_library(
    name = "lexer",
    srcs = [
        "lexer.cc",
    ],
    hdrs = [
        "lexer.h",
    ],
    include_prefix = "warren/json/internal/parse",
    strip_include_prefix = ".",
    visibility = [
        "//src:__subpackages__",
        "//tests:__subpackages__",
    ],
    deps = [
        ":token",
    ],
)

cc_library(
    name = "token",
    hdrs = [
        "token.h",
    ],
    include_prefix = "warren/json/internal/parse",
    strip_include_prefix = ".",
    visibility = [
        "//src:__subpackages__",
        "//tests:__subpackages__",
    ],
)
