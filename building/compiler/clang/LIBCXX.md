# Using libc++

[Libc++](https://libcxx.llvm.org/UsingLibcxx.html) implements the various versions of the C++ Standard. Changing the version of the standard can be done by passing `-std=c++XY` to the compiler. Libc++ will automatically detect what Standard is being used and will provide functionality that matches that Standard in the library.

```bash
~ $ clang++ -std=c++20 hello.cpp
~ $ ./a.out
Hello, world!
```

On systems where libc++ is provided but is not the default, Clang provides a flag called `-stdlib=` that can be used to decide which standard library is used. Using `-stdlib=libc++` will select libc++:

```bash
~ $ clang++ -std=c++20 -stdlib=libc++ hello.cpp
~ $ ./a.out
./a.out: error while loading shared libraries: libc++.so.1: cannot open shared object file: No such file or directory

~ $ export LD_LIBRARY_PATH=/usr/local/clang-16.0.0/lib/aarch64-unknown-linux-gnu
~ $ ./a.out
Hello, world!
```

On systems where libc++ is the library in use by default such as macOS and FreeBSD, this flag is not required.

> While [`LD_LIBRARY_PATH`](https://tecadmin.net/understanding-the-ld_library_path-environment-variable/) can be handy, an attacker can set LD_LIBRARY_PATH to a directory containing malicious versions of common libraries, tricking the program into using them.

Clang provides a way to disable the default behavior for finding the standard library and to override it with custom paths.

```bash
~ $ unset LD_LIBRARY_PATH
~ $ ./a.out
./a.out: error while loading shared libraries: libc++.so.1: cannot open shared object file: No such file or directory

~ $ clang++ -std=c++20 -stdlib=libc++ \
-Wl,-rpath,/usr/local/clang-16.0.0/lib/aarch64-unknown-linux-gnu hello.cpp
~ $ ./a.out
Hello, world!
```

> Thus, the first example `clang++ -std=c++20 hello.cpp` seems to link GCC `stdlib++`, as GCC is the default C/C++ compiler of Debian Bullseye. 

```bash
~ $ clang++ -std=c++20 hello.cpp
~ $ file a.out
a.out: ELF 64-bit LSB pie executable, ARM aarch64, version 1 (SYSV), dynamically linked, interpreter /lib/ld-linux-aarch64.so.1, for GNU/Linux 3.7.0, not stripped
~ $ ls -l /lib/ld-linux-aarch64.*
lrwxrwxrwx 1 root root 28 Oct 18  2022 /lib/ld-linux-aarch64.so.1 -> aarch64-linux-gnu/ld-2.31.so
~ $ ls -l /lib/aarch64-linux-gnu/ld-2.31.so
-rwxr-xr-x 1 root root 145352 Oct 18  2022 /lib/aarch64-linux-gnu/ld-2.31.so
```