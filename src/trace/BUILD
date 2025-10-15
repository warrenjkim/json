cc_library(
    name = "trace",
    include_prefix = "warren/json/trace",
    visibility = ["//visibility:public"],
    deps = [
        ":attribute",
        ":event",
        ":null_tracer",
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
