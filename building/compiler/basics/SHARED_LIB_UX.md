# Shared Libraries on Unix

[Shared libraries](https://tldp.org/HOWTO/Program-Library-HOWTO/shared-libraries.html) are libraries that are loaded by programs when they start. When a shared library is installed properly, all programs that start afterwards automatically use the new shared library.

## Shared library names

Here is the difference between a library's names, in particular its *real name*, *soname*, and *linker name*:

A real name is the filename containing the actual library code.

```
real name   := lib(name).so.version.minor.release
```

A fully-qualified soname includes as a prefix the directory it's in; on a working system a fully-qualified soname is simply a symbolic link to the shared library's real name.

```
soname      := lib(name).so.version
```

The name that the compiler uses when requesting a library (linker name) is simply the soname without any version number.

```
linker name := lib(name).so
```

Programs, when they internally list the shared libraries they need, should only list the soname they need.

```bash
consuming $ ldd shared/build/compressor
	linux-vdso.so.1 (0x...)
	libz.so.1 => /home/.../.conan2/.../libz.so.1 (0x...)
	libc++.so.1 => not found
	libc++abi.so.1 => not found
	libm.so.6 => /lib/.../libm.so.6 (0x...)
	libc.so.6 => /lib/.../libc.so.6 (0x...)
	/lib/ld-linux-aarch64.so.1 (-x...)
```

## Creating a shared library

The source code files are:

```c++
// say_hello.h
#include <string>
std::string say_hello(std::string name);

// say_hello.cpp
#include "say_hello.h"
std::string say_hello(std::string name) {
  return "Hello, " + name;
}
```

This example creates one object file (`say_hello.o`) and then creates a shared library (`hello.so.1.0.1`) that contains the object:

```bash
cpp $ clang++ -std=c++20 -fPIC -g -c -Wall say_hello.cpp

cpp $ clang++ -shared \
  -Wl,-soname,libhello.so.1 \
  -o libhello.so.1.0.1 \
  say_hello.o -lc++

cpp $ ls
-rwxr-xr-x 1 ... ...  75960 Jul 30 19:45 libhello.so.1.0.1
-rw-r--r-- 1 ... ...     94 Jul 30 14:00 say_hello.cpp
-rw-r--r-- 1 ... ...     59 Jul 30 13:58 say_hello.h
-rw-r--r-- 1 ... ... 100192 Jul 30 19:45 say_hello.o
```

Note that:

- This compilation includes debugging information (`-g`) and will generate warnings (`-Wall`), which aren't required for shared libraries but are recommended.
- Use `-fPIC` or `-fpic` to generate code (PIC: position independent code). Whether to use `-fPIC` or `-fpic` to generate code is target-dependent.

  - The `-fPIC` choice always works, but may produce larger code than `-fpic`.
  - Using `-fpic` option usually generates smaller and faster code, but will have platform-dependent limitations, such as the number of globally visible symbols or the size of the code.

  The linker will tell you whether it fits when you create the shared library. When in doubt, I choose `-fPIC`, because it always works.

## Installing a shared library

To [add new shared libraries](https://tldp.org/HOWTO/Program-Library-HOWTO/more-examples.html) to the system,

(1) place them into a directory:

```bash
cpp $ sudo mkdir /usr/local/lib/hello
cpp $ sudo cp libhello.so.1.0.1 /usr/local/lib/hello/
```

(2) set up the soname:

```bash
cpp $ cd /usr/local/lib/hello/
hello $ sudo ln -sf libhello.so.1.0.1 libhello.so.1
```

(3) set up the linker name:

```bash
hello $ sudo ln -sf libhello.so.1 libhello.so
```

The library directory now has three objects:

```bash
hello $ ls -l
lrwxrwxrwx 1 root root 13 ... libhello.so -> libhello.so.1
lrwxrwxrwx 1 root root 17 ... libhello.so.1 -> libhello.so.1.0.1
-rwxr-xr-x 1 root root .. ... libhello.so.1.0.1

hello $ cd path/to/cpp
```

## Using shared libraries

Here is an example program that use the shared library:

```c++
#include "say_hello.h"
#include <iostream>
int main() {
    std::cout << say_hello("world!\n");
}
```

Thanks to the linker name `libhello.so` in the library directory, you can compile the program with the shared library:

```bash
cpp $ clang++ -std=c++20 \
  -L/usr/local/lib/hello -lhello -lc++ \
  hello.cpp
```

To run the program, set the `LD_LIBRARY_PATH` environment variable:

```bash
cpp $ export LD_LIBRARY_PATH=/usr/local/clang-16.0.0/lib/aarch64-unknown-linux-gnu:/usr/local/lib/hello

cpp $ ./a.out
Hello, world!

cpp $ export LD_LIBRARY_PATH=/usr/local/clang-16.0.0/lib/aarch64-unknown-linux-gnu

cpp $ ./a.out
./a.out: error while loading shared libraries: libhello.so.1: cannot open shared object file: No such file or directory
```

Notice that the process to load the shared library refers to the soname `libhello.so.1`.
