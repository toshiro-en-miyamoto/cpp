# Stage 1

This showcases how to build a single file to create a runnable application.

This BUILD file shows that we want to build a C++ binary using the `cc_binary` rule provided by Bazel. In the `cc_binary` rule, name of the binary is specified in `name` attribute (in this example, it's `hello-world`), required source files to be built are provided in `srcs` attribute.

```
load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "hello-world",
    srcs = ["hello-world.cc"],
    copts = ["-std=c++20"],
)
```

To build this example,

```bash
$ cd <path-to-workspace>

$ bazel build stage1/main:hello-world
Extracting Bazel installation...
Starting local Bazel server and connecting to it...
INFO: Analyzed target //stage1/main:hello-world (37 packages loaded, 165 targets configured).
INFO: Found 1 target...
Target //stage1/main:hello-world up-to-date:
  bazel-bin/stage1/main/hello-world
INFO: Elapsed time: 83.773s, Critical Path: 3.25s
INFO: 6 processes: 4 internal, 2 linux-sandbox.
INFO: Build completed successfully, 6 total actions
```
