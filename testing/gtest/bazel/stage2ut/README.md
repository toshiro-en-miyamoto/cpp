## Writing and running C++ tests

For example, you could create a test `./test/hello-test.cc`, such as:

```C++
#include <gtest/gtest.h>
#include "stage2ut/main/hello-greet.h"

TEST(HelloTest, GetGreet) {
    EXPECT_EQ(get_greet("Bazel"), "Hello Bazel");
}
```

Then create `./test/BUILD` file for your tests:

```
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
```

This `cc_test` rule declares the C++ test binary you want to build, and links to GoogleTest (a magic keyword `//:gtest_main`) using the prefix specified in the `WORKSPACE` file (`@com_google_googletest`). The `size = "small"` argument prevents a message like `specified size is too big` from being displayed by `bazel test` command.

## Making packages visible

To make `hello-greet` visible to `hello-test`, you must add the `visibility` attribute.

```
load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_library(
    name = "hello-greet",
    srcs = ["hello-greet.cc"],
    hdrs = ["hello-greet.h"],
    copts = ["-std=c++20"],
    visibility = ["//stage2ut/test:__pkg__"],
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

## Running tests

Now you can use `bazel test` to run the test.

```bash
$ cd <path-to-workspace>

$ bazel test stage2ut/test:hello-test
INFO: Analyzed target //stage2ut/test:hello-test (0 packages loaded, 0 targets configured).
INFO: Found 1 test target...
Target //stage2ut/test:hello-test up-to-date:
  bazel-bin/stage2ut/test/hello-test
INFO: Elapsed time: 39.534s, Critical Path: 17.27s
INFO: 30 processes: 8 internal, 22 linux-sandbox.
INFO: Build completed successfully, 30 total actions
//stage2ut/test:hello-test                                 PASSED in 0.1s

INFO: Build completed successfully, 30 total actions
```
