cc_test(
    name = "hello-test",
    size = "small",
    srcs = ["hello-test.cc"],
    copts = ["-std=c++20"],
    deps = [
        "@com_google_googletest//:gtest_main",
        "//stage2ut/main:hello-greet",
    ],
)
