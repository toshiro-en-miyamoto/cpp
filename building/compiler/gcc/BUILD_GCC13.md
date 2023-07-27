# Building GCC 13

[This document](https://gcc.gnu.org/install/) describes the generic installation procedure for GCC.

## Downloading the source

GCC is distributed [via git](https://gcc.gnu.org/git.html).

```bash
work $ git clone git://gcc.gnu.org/git/gcc.git
```

If you want to build a gcc snapshot or gcc from git tree you will need to add `flex` as an explicit dependency into your derivation.

```bash
work $ sudo apt install flex
```

You can get a list of recent tags available in the repository:

```bash
work $ gcc
gcc $ git tag -l | sed -n '/^releases\/gcc-1[0-9]/p'
releases/gcc-10.1.0
releases/gcc-10.2.0
releases/gcc-10.3.0
releases/gcc-10.4.0
releases/gcc-10.5.0
releases/gcc-11.1.0
releases/gcc-11.2.0
releases/gcc-11.3.0
releases/gcc-11.4.0
releases/gcc-12.1.0
releases/gcc-12.2.0
releases/gcc-12.3.0
releases/gcc-13.1.0
```

To build GCC 13.1.0, switch to the `gcc-13.1.0`:

```bash
gcc $ git checkout releases/gcc-13.1.0
Updating files: 100% (7331/7331), done.
Note: switching to 'releases/gcc-13.1.0'.
```

Several support libraries are [necessary](https://gcc.gnu.org/install/prerequisites.html) to build GCC.

```bash
gcc $ ./contrib/download_prerequisites
2023-07-26 21:32:19 URL:http://gcc.gnu.org/pub/gcc/infrastructure/gmp-6.2.1.tar.bz2 [2493916/2493916] -> "gmp-6.2.1.tar.bz2" [1]
2023-07-26 21:32:24 URL:http://gcc.gnu.org/pub/gcc/infrastructure/mpfr-4.1.0.tar.bz2 [1747243/1747243] -> "mpfr-4.1.0.tar.bz2" [1]
2023-07-26 21:32:27 URL:http://gcc.gnu.org/pub/gcc/infrastructure/mpc-1.2.1.tar.gz [838731/838731] -> "mpc-1.2.1.tar.gz" [1]
2023-07-26 21:32:33 URL:http://gcc.gnu.org/pub/gcc/infrastructure/isl-0.24.tar.bz2 [2261594/2261594] -> "isl-0.24.tar.bz2" [1]
gmp-6.2.1.tar.bz2: OK
mpfr-4.1.0.tar.bz2: OK
mpc-1.2.1.tar.gz: OK
isl-0.24.tar.bz2: OK
All prerequisites downloaded successfully.
```

## Configuration

Like most GNU software, GCC must be [configured](https://gcc.gnu.org/install/configure.html) before it can be built.

We use `gcc` to refer to the top-level source directory for GCC; we use `build` to refer to the top-level build/object directory.

It is highly recommended that GCC be built into a separate directory from the sources which does not reside within the source tree.

```bash
gcc $ cd ..
work $ ls
gcc
work $ mkdir build
work $ cd build
build $
```

To [configure](https://gcc.gnu.org/install/configure.html) GCC:

```bash
build $ ../gcc/configure [options] [target]
```

*Host, Build and Target specification* specifies the host, build and target machine configurations.

- The *build* machine is the system which you are using,
- the *host* machine is the system where you want to run the resulting compiler (normally the build machine), and
- the *target* machine is the system for which you want the compiler to generate code.

> If you are building a compiler to produce code for the machine it runs on (a native compiler), specify the build machine’s configuration name with the `--host` option; the host and target will default to be the same as the host machine.

> With Raspberry Pi OS, we specify `--host=aarch64-linux-gnu`, it would be `aarch64-unknown-linux-gnu` otherwise.

`--prefix=dirname` specifies the top-level installation directory. This is the recommended way to install the tools into a directory other than the default. The top-level installation directory defaults to `/usr/local`.

> We specify `/usr/local/gcc-13.1.0`.

`--enable-languages=lang1,lang2,…` specifies that only a particular subset of compilers and their runtime libraries should be built.

> We specify `c,c++`.

Run `configure`:

```bash
build $ ../gcc/configure \
  --host=aarch64-linux-gnu \
  --prefix=/usr/local/gcc-13.1.0 \
  --enable-languages=c,c++

......
configure: creating ./config.status
config.status: creating Makefile

build $ grep '^host_alias' Makefile
host_alias=aarch64-linux-gnu
build $ grep '^target_alias' Makefile
target_alias=aarch64-linux-gnu
```

Now we build GCC:

```bash
build $ make -j 5
```

Started 01:07, July 27

## Final install

