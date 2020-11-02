load("@rules_cc//cc:defs.bzl", "cc_library")

OPT0FLAG = "-O0"
OPT1FLAG = "-O1"
OPT2FLAG = "-O2"
OPT3FLAG = "-O3"
OPTNFLAG = OPT3FLAG

cc_library(
    name = "xparser",
    hdrs = glob(["inc/**/*.h"]),
    srcs = glob(["src/**/*.cpp"]),
    copts = [
        "-std=c++17",
        "-Werror",
        "-Wall",
        "-Wextra",
        "-Wunused",
        "-Wunreachable-code",
        "-fPIC",
        "-momit-leaf-frame-pointer",
        "-g", "-v",
        OPTNFLAG,

        "-Wnon-virtual-dtor",
        "-Wmissing-declarations",
        "-Wunreachable-code",
    ],
    strip_include_prefix = "/lib/inc",
    visibility = ["//main:__pkg__", "//test:__pkg__"],
)
