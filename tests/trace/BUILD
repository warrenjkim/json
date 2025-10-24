test_suite(
    name = "tracer_tests",
    tests = [
        ":in_memory_tracer_test",
    ],
)

cc_test(
    name = "in_memory_tracer_test",
    srcs = ["in_memory_tracer_test.cc"],
    deps = [
        "//src/trace:in_memory_tracer",
        "@googletest//:gtest_main",
    ],
)
