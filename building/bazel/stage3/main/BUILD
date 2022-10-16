load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_library(
    name = "hello-greet",
    srcs = ["hello-greet.cc"],
    hdrs = ["hello-greet.h"],
    copts = ["-std=c++20"],
)

cc_binary(
    name = "hello-world",
    srcs = ["hello-world.cc"],
    copts = ["-std=c++20"],
    deps = [
        ":hello-greet",
        "//stage3/lib:hello-time",
    ],
)
