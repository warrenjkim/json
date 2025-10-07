cc_library(
    name = "trace",
    include_prefix = "warren/json/trace",
    visibility = ["//visibility:public"],
    deps = [
        ":attribute",
        ":event",
        ":null_span",
        ":null_tracer",
        ":span",
        ":tracer",
    ],
)

cc_library(
    name = "null_tracer",
    hdrs = [
        "null_tracer.h",
    ],
    include_prefix = "warren/json/trace",
    visibility = ["//visibility:public"],
    deps = [
        ":null_span",
        ":tracer",
    ],
)

cc_library(
    name = "tracer",
    hdrs = [
        "tracer.h",
    ],
    include_prefix = "warren/json/trace",
    visibility = ["//visibility:public"],
)

cc_library(
    name = "null_span",
    hdrs = [
        "null_span.h",
    ],
    include_prefix = "warren/json/trace",
    visibility = ["//visibility:public"],
    deps = [
        ":span",
    ],
)

cc_library(
    name = "span",
    hdrs = [
        "span.h",
    ],
    include_prefix = "warren/json/trace",
    visibility = ["//visibility:public"],
)

cc_library(
    name = "attribute",
    hdrs = [
        "attribute.h",
    ],
    include_prefix = "warren/json/trace",
    visibility = ["//visibility:public"],
)

cc_library(
    name = "event",
    hdrs = [
        "event.h",
    ],
    include_prefix = "warren/json/trace",
    visibility = ["//visibility:public"],
)
