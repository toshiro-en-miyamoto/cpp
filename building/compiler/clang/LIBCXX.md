# Using libc++

[Libc++](https://libcxx.llvm.org/UsingLibcxx.html) implements the various versions of the C++ Standard. Changing the version of the standard can be done by passing `-std=c++XY` to the compiler. Libc++ will automatically detect what Standard is being used and will provide functionality that matches that Standard in the library.

```bash
~  $ which clang++
/usr/local/clang-16.0.0/bin/clang++

~ $ clang++ -std=c++20 hello.cpp
~ $ ./a.out
Hello, world!
```

On systems where libc++ is provided but is not the default, Clang provides a flag called `-stdlib=` that can be used to decide which standard library is used. Using `-stdlib=libc++` will select libc++:

```bash
~ $ clang++ -std=c++20 -lc++ hello.cpp
~ $ ./a.out
./a.out: error while loading shared libraries: libc++.so.1: cannot open shared object file: No such file or directory

~ $ export LD_LIBRARY_PATH=/usr/local/clang-16.0.0/lib/aarch64-unknown-linux-gnu
~ $ ./a.out
Hello, world!
```

As shown above, systems with GCC as the default compiler don't know where `libc++.so` resides. You need to let the system know about it; setting `LD_LIBRARY_PATH` is one of the options.

> While [`LD_LIBRARY_PATH`](https://tecadmin.net/understanding-the-ld_library_path-environment-variable/) can be handy, an attacker can set LD_LIBRARY_PATH to a directory containing malicious versions of common libraries, tricking the program into using them.

Apple Mac OS, on the other hand, is provided with Clang as the default compiler. Therefore, `-lc++` simple works.

```bash
~ $ clang++ -std=c++20 -lc++ hello.cpp
~ $ ./a.out
Hello, world!
```

On systems where libc++ is the library in use by default such as macOS and FreeBSD, this flag is not required.

With dynamic linking on GCC-defaulted systems, Clang provides a way to disable the default behavior for finding the standard library and to override it with custom paths.

```bash
~ $ unset LD_LIBRARY_PATH
~ $ ./a.out
./a.out: error while loading shared libraries: libc++.so.1: cannot open shared object file: No such file or directory

~ $ clang++ -std=c++20 -lc++ \
-Wl,-rpath,/usr/local/clang-16.0.0/lib/aarch64-unknown-linux-gnu \ hello.cpp
~ $ ./a.out
Hello, world!
```

Static link would help on GCC-defaulted systems:

```bash
~ $ clang++ -std=c++20 -static -lc++ hello.cpp
~ $ ./a.out
Hello, world!
```

On Mac OS, however, it won't work, as Apple does not support *statically linked binaries* on Mac OS X.

```bash
~ $ clang++ -std=c++20 -static -lc++ hello.cpp
ld: library not found for -lcrt0.o

~ $ clang++ -std=c++20 -static hello.cpp 
ld: library not found for -lcrt0.o
```

## Statically linked binaries on Mac OS X

[Technical Q&A QA1118](https://developer.apple.com/library/archive/qa/qa1118/_index.html)

Q:  I'm trying to link my binary statically, but it's failing to link because it can't find `crt0.o`. Why?

A: Before discussing this issue, it's important to be clear about terminology:

- A *static library* is a library of code that can be linked into a binary that will, eventually, be dynamically linked to the system libraries and frameworks.
- A *statically linked binary* is one that does not import system libraries and frameworks dynamically, but instead makes direct system calls into the kernel.

Apple fully supports static libraries; if you want to create one, just start with the appropriate Xcode project or target template.

**Apple does not support statically linked binaries on Mac OS X**. A statically linked binary assumes binary compatibility at the kernel system call interface, and we do not make any guarantees on that front. Rather, we strive to ensure binary compatibility in each dynamically linked system library and framework.

If your project absolutely must create a statically linked binary, you can get the `Csu` (C startup) module from Darwin and try building `crt0.o` for yourself. Obviously, we won't support such an endeavor.

> Warning: Shipping a statically linked binary entails a significant compatibility risk. We strongly recommend that you not do this.

## Conclusion

With Clang, choose dynamic linking:

- pros:
  - it enables cross-platform compiling include Apple Mac OS X
- cons:
  - it requires you specify where `libc++.so` is located

The issue of the location of `libc++.so` applies only to GCC-defaulted systems, where you have options:

- on developers' machine, use `LD_LIBRARY_PATH`
- on operations servers, maintain `/etc/ld.so.conf`
