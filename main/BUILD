load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "calendar-example",
    srcs = ["calendarExample.cpp"],
    deps = [
        "//lib:xparser",
    ],
    copts = [
        "-std=c++17",
        "-g",
    ],
    linkstatic = 0,
)
