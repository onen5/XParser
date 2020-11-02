
load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "xparser-test",
    srcs = glob(["unittest/*.cpp"]),
    copts = ["-Iexternal/gtest/include"],
    deps = [
        "@gtest//:gtest_main",
        "//lib:xparser",
    ],
)