cc_library(
    name = "api",
    include_prefix = "warren/json",
    strip_include_prefix = ".",
    visibility = ["//visibility:public"],
    deps = [
        ":parse",
        ":value",
    ],
)

cc_library(
    name = "value",
    hdrs = [
        "value.h",
    ],
    include_prefix = "warren/json",
    strip_include_prefix = ".",
    visibility = ["//visibility:public"],
    deps = [
        "//src/utils:exception",
        "//src/utils:types",
    ],
)

cc_library(
    name = "parse",
    hdrs = [
        "parse.h",
    ],
    include_prefix = "warren/json",
    strip_include_prefix = ".",
    visibility = ["//visibility:public"],
    deps = [
        ":value",
        "//src/parse:lexer",
        "//src/parse:parser",
        "//src/utils:types",
    ],
)
