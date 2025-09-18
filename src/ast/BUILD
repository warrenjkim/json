cc_library(
    name = "ast",
    include_prefix = "warren/json/internal/ast",
    strip_include_prefix = ".",
    visibility = [
        "//src:__subpackages__",
        "//tests:__subpackages__",
    ],
    deps = [
        ":nodes",
        ":visitor",
    ],
)

cc_library(
    name = "nodes",
    hdrs = [
        "array.h",
        "boolean.h",
        "node.h",
        "null.h",
        "number.h",
        "object.h",
        "string.h",
    ],
    include_prefix = "warren/json/internal/ast",
    strip_include_prefix = ".",
    visibility = [
        "//src:__subpackages__",
        "//tests:__subpackages__",
    ],
    deps = [
        ":visitor",
    ],
)

cc_library(
    name = "visitor",
    hdrs = [
        "visitor.h",
    ],
    include_prefix = "warren/json/internal/ast",
    strip_include_prefix = ".",
    visibility = [
        "//src:__subpackages__",
        "//tests:__subpackages__",
    ],
)
