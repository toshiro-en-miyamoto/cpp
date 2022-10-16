# Installing Bazel

Bazelisk is the recommended way to install Bazel on Ubuntu Linux, macOS, and Windows.

## [Installing Bazel using Bazelisk](https://bazel.build/install/bazelisk)

[Bazelisk](https://github.com/bazelbuild/bazelisk/blob/master/README.md) automatically downloads and installs the appropriate version of Bazel. You can install Bazelisk in multiple ways, including:

- using a binary release for Linux, macOS, or Windows
- using npm: npm install -g @bazel/bazelisk

Or
- On macOS: `brew install bazelisk`
- On Windows: `choco install bazelisk`
- Each adds bazelisk to the `PATH` as both `bazelisk` and `bazel`.

## [Binary releases](https://github.com/bazelbuild/bazelisk/releases)

1. Download a binary release such as `bazelisk-linux-arm64` of `v1.11.0`.
2. Copy the binary as `bazel` in you `PATH`.
```bash
[~/Downloads] % sudo cp bazelisk-linux-arm64 /usr/local/bin/bazel
[/usr/local/bin] % sudo chmod +x bazel
```
