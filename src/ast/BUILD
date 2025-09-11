cc_library(
    name = "ast",
    hdrs = [
        "array.h",
        "boolean.h",
        "node.h",
        "null.h",
        "number.h",
        "object.h",
        "string.h",
        "visitor.h",
    ],
    include_prefix = "warren/json/internal/ast",
    strip_include_prefix = ".",
    visibility = [
        "//src:__subpackages__",
        "//tests:__subpackages__",
    ],
    deps = [
        "//src/utils",
    ],
)
