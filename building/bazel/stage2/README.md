# Stage 2

Here, we introduce the `cc_library` rule for building C++ libraries. We have a `cc_library` named `hello-greet` and its header and source files are defined accordingly.

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
    ],
)
```

The `cc_binary` rule we saw in stage 1 has not changed, except that we now depend on the library `hello-greet`.

To build this example,

```bash
$ cd <path-to-workspace>

$ bazel build stage2/main:hello-world
INFO: Analyzed target //stage2/main:hello-world (1 packages loaded, 5 targets configured).
INFO: Found 1 target...
Target //stage2/main:hello-world up-to-date:
  bazel-bin/stage2/main/hello-world
INFO: Elapsed time: 3.380s, Critical Path: 2.26s
INFO: 7 processes: 4 internal, 3 linux-sandbox.
INFO: Build completed successfully, 7 total actions
```
