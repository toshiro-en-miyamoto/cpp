## Create and run a binary

With your Bazel workspace set up, you can now use GoogleTest code within your own project. As an example, create a file named `hello_test.cc` in your workspace directory with the following contents:

```c++
#include <gtest/gtest.h>

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}
```

To build the code, create a file named `BUILD` in the same directory with the following contents:

```
cc_test(
    name = "hello-test",
    size = "small",
    srcs = ["hello_test.cc"],
    deps = ["@com_google_googletest//:gtest_main"],
    copts = ["-std=c++20"],
)
```

This `cc_test` rule declares the C++ test binary you want to build, and links to GoogleTest (`//:gtest_main`) using the prefix you specified in the `WORKSPACE` file (`@com_google_googletest`).

Now you can build and run your test:

```bash
$ cd <path-to-workspace>

$ bazel test gtest:hello-test
INFO: Analyzed target //gtest:hello-test (1 packages loaded, 2 targets configured).
INFO: Found 1 test target...
Target //gtest:hello-test up-to-date:
  bazel-bin/gtest/hello-test
INFO: Elapsed time: 6.309s, Critical Path: 5.59s
INFO: 7 processes: 4 internal, 3 linux-sandbox.
INFO: Build completed successfully, 7 total actions
//gtest:hello-test                                                       PASSED in 0.1s

INFO: Build completed successfully, 7 total actions
```
