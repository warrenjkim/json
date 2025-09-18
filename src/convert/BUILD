cc_library(
    name = "convert",
    srcs = [
        "lower.cc",
    ],
    hdrs = [
        "lower.h",
    ],
    include_prefix = "warren/json/internal/convert",
    strip_include_prefix = ".",
    visibility = [
        "//src:__subpackages__",
        "//tests:__subpackages__",
    ],
    deps = [
        "//src/api:value",
        "//src/ast:nodes",
        "//src/ast:visitor",
        "//src/utils:exception",
    ],
)
