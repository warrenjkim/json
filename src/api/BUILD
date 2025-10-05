cc_library(
    name = "api",
    include_prefix = "warren/json",
    strip_include_prefix = ".",
    visibility = ["//visibility:public"],
    deps = [
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
    ],
)
