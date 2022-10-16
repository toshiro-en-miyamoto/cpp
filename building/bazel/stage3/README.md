# Stage 3

In this stage we step it up and showcase how to integrate multiple `cc_library` targets from different packages.

Below, we see a similar configuration from Stage 2, except that this BUILD file is in a subdirectory called lib. In Bazel, subdirectories containing BUILD files are known as packages. The new property `visibility` will tell Bazel which package(s) can reference this target, in this case the `//stage3/main` package can use `hello-time` library. 

```
load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "hello-time",
    srcs = ["hello-time.cc"],
    hdrs = ["hello-time.h"],
    visibility = ["//stage3/main:__pkg__"],
)
```

To use our `hello-time` library, an extra dependency is added in the form of //path/to/package:target_name, in this case, it's `//stage3/lib:hello-time`

```
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
```

To build this example,

```bash
$ cd <path-to-workspace>

$ bazel build stage3/main:hello-world
INFO: Analyzed target //stage3/main:hello-world (0 packages loaded, 0 targets configured).
INFO: Found 1 target...
Target //stage3/main:hello-world up-to-date:
  bazel-bin/stage3/main/hello-world
INFO: Elapsed time: 2.948s, Critical Path: 2.47s
INFO: 5 processes: 1 internal, 4 linux-sandbox.
INFO: Build completed successfully, 5 total actions
```
