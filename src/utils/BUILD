cc_library(
    name = "utils",
    include_prefix = "warren/json/utils",
    visibility = ["//visibility:public"],
    deps = [
        ":exception",
        ":parse",
        ":to_string",
        ":types",
    ],
)

cc_library(
    name = "parse",
    hdrs = [
        "parse.h",
    ],
    include_prefix = "warren/json/utils",
    strip_include_prefix = ".",
    visibility = ["//visibility:public"],
    deps = [
        ":types",
        "//src/api:value",
        "//src/parse:lexer",
        "//src/parse:parser",
    ],
)

cc_library(
    name = "to_string",
    srcs = [
        "to_string.cc",
    ],
    hdrs = [
        "to_string.h",
    ],
    include_prefix = "warren/json/utils",
    strip_include_prefix = ".",
    visibility = ["//visibility:public"],
    deps = [
        ":types",
        "//src/api:value",
    ],
)

cc_library(
    name = "exception",
    hdrs = [
        "exception.h",
    ],
    include_prefix = "warren/json/utils",
    visibility = ["//visibility:public"],
)

cc_library(
    name = "types",
    hdrs = [
        "types.h",
    ],
    include_prefix = "warren/json/utils",
    visibility = ["//visibility:public"],
)
