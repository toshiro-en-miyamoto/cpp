# Unit Testing with Google Test

This is an excerpt from [Google Test Quick Start: Bazel](https://google.github.io/googletest/quickstart-bazel.html) and [Bazel Common C++ Build Use Cases](https://bazel.build/tutorials/cpp-use-cases).

## Set up a Bazel workspace

Create the `WORKSPACE` file to specify dependencies. A common and recommended way to depend on GoogleTest is to use a Bazel external dependency via the `http_archive` rule. To do this, in the root directory of your workspace, create a file named `WORKSPACE` with the following contents:

```
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "com_google_googletest",
    urls = ["https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip"],
    strip_prefix = "googletest-release-1.12.1"
)
```

This `http_archive` rule declares a dependency on GoogleTest which is downloaded as a ZIP archive from GitHub. You can find the ZIP file in the `asset` section, the very last section of each release at the [GoogleTest release site](https://github.com/google/googletest/releases). The latest release is recommended.

The value of `sha256` argument can be obtained by executing `bazel test` command without the argument, which suggests the `sha256` value in its DEBUG messages.

In `strip_prefix` argument, a magic keyworkd `googletest-release-` must be used *as is*.

Once you run `bazel test` command, its DEBUG message suggests the `sha256` value. Append the value in the `http_archive` rule to dismiss the DEBUG message.

```
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "com_google_googletest",
    urls = ["https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip"],
    strip_prefix = "googletest-release-1.12.1",
    sha256 = "24564e3b712d3eb30ac9a85d92f7d720f60cc0173730ac166f27dda7fed76cb2"
)
```

