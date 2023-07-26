# Building GCC 12

## The filesystem hierarchy

Reference: [The filesystem hierarchy](https://www.man7.org/linux/man-pages/man7/hier.7.html)

| dir | description
|-----|-------------
| `/bin` | Contains executable programs which are needed in single user mode and to bring the system up or repair it.
| `/usr` | Should hold only shareable, read-only data, so that it can be mounted by various machines running Linux.
| `/usr/bin` | Is the primary directory for executable programs. Most programs executed by normal users which are not needed for booting or for repairing the system and which are not installed locally should be placed in this directory.
| `/usr/include` | Includes files for the C compiler.
| `/usr/include/g++` | Includes files to use with the GNU C++ compiler.
| `/usr/local` | Is where programs which are local to the site typically go.
| `/usr/local/bin` | Includes binaries for programs local to the site.
| `/usr/local/include` | Includes header files for the local C compiler.

## Filesystem requirements

1.7GB of free space at minimum is required to build GCC 12.2 C++.

| used (KB)     | difference    | upon
|--------------:|---------------|------
|          0    |  4,074,696    | OS & VS Code installed
| 14,265,580    | 10,190,884    | GCC/lib source in place
| 15,770,612    |  1,505,032    | Build completed, c++ installed
|  5,653,628    | 10,116,984    | source & build deleted

## Configuration of GCC 10.2.1

For example, with Debian Bullseye 10.2:

```
$ lsb_release -a
No LSB modules are available.
Distributor ID: Debian
Description:    Debian GNU/Linux 11 (bullseye)
Release:        11
Codename:       bullseye

$ g++ --version
g++ (Debian 10.2.1-6) 10.2.1 20210110

$ which g++
/usr/bin/g++
```

Configuration of `cpp`:

```
$ cpp -v /dev/null -o /dev/null
Using built-in specs.
COLLECT_GCC=cpp
Target: aarch64-linux-gnu
Configured with: ../src/configure -v
    --see below
Thread model: posix
Supported LTO compression algorithms: zlib zstd
```

Configuration of `g++`:

```
$ g++ -v /dev/null -o /dev/null
Using built-in specs.
COLLECT_GCC=g++
COLLECT_LTO_WRAPPER=/usr/lib/gcc/aarch64-linux-gnu/10/lto-wrapper
Target: aarch64-linux-gnu
Configured with: ../src/configure -v
    --see below
Thread model: posix
Supported LTO compression algorithms: zlib zstd
```

It turns out that `cpp` and `g++` have the same configuration.

## Prerequisites

Reference: [General prerequisites for GCC](https://gcc.gnu.org/install/prerequisites.html)

Required compilers include:

- C++11 compiler
- C standard library and headers

Required tools include:

- `bash` (`zsh` will not work)
- `awk`
- `binutils` 2.35 or newer is required for LTO
- `gzip` 1.2.4 (or later) or `bzip2` 1.0.2 (or later)
- `make`
- `tar`
- `python3`

Debian Bullseye 10.2 installs the tools by default.

| pack name  | ver    | description
|------------|--------|-------------
| `bash`     | 5.1.2  | GNU Bourne Again SHell
| `gawk`     | 5.1.0  | GNU awk
| `binutils` | 2.35.2 | GNU assembler, linker and binary utilities
| `gzip`     | 1.10   | GNU compression utilities
| `make`     | 4.3    | utility for directing compilation
| `tar`      | 1.34   | GNU version of the tar archiving utility
| `python3`  | 3.9.2  | interactive high-level object-oriented language

Required libraries include:

- GMP version 4.3.2 (or later)
- MPFR Library version 3.1.0 (or later)
- MPC Library version 1.0.1 (or later)
- `isl` Library version 0.15 or later
- `zstd` Library

Debian Bullseye 10.2 installs the libraries by default.

| pack name  | ver   | description
|------------|-------|-------------
| `libgmp10` | 6.2.1 | Multi-precision arithmetic library
| `libmpfr6` | 4.1.0 | multiple precision floating-point computation
| `libmpc3`  | 1.2.0 | multiple precision complex floating-point library
| `libisl23` | 0.23  | manipulating sets and relations of integer points
| `libzstd1` | 1.4.8 | fast lossless compression algorithm

In addition to the libraries above, the following source distributions are required but not installed by default. Without them, `configure` tells like:

```
checking for the correct version of gmp.h... no
configure: error: Building GCC requires GMP 4.2+, MPFR 3.1.0+ and MPC 0.8.0+.
```

Let's download the latest version of the source distributions, and place them in the GCC sources:

| DIR    | tar file     | URL to download
|--------|--------------|-----------------
| `gmp`  | `gmp-6.2.1`  | https://gmplib.org/#DOWNLOAD
| `mpfr` | `mpfr-4.2.0` | https://www.mpfr.org/mpfr-current/
| `mpc`  | `mpc-1.3.1`  | https://www.multiprecision.org/mpc/download.html
| `isl`  | `isl-0.24`   | https://gcc.gnu.org/pub/gcc/infrastructure/

## Configuration

[Installing GCC: Configuration](https://gcc.gnu.org/install/configure.html)

- `src-dir`: the top-level source directory
- `obj-dir`: the top-level build/object directory

Recommending `src-dir != obj-dir`. Building where `obj-dir` is a subdirectory of `src-dir` is unsupported.

`cc` or `gcc` must be in your path.

To configure GCC:

```
% mkdir obj-dir
% cd obj-dir
% src-dir/configure [options] [target]
```

### Distributor options

If you will be distributing binary versions of GCC, with modifications to the source code, you should use the *distributor options* to make clear that your version contains modifications.

- `with-pkgversion`
  - This version string will be included in the output of `gcc --version`. The default value is `GCC`.
  - Deb: `Debian 10.2.1-6`
  - pi4: N/A (`GCC`)
- `with-bugurl`
  - Specify the URL that users should visit if they wish to report a bug. The default value refers to the FSF’s GCC bug tracker.
  - Deb: `file:///usr/share/doc/gcc-10/README.Bugs`
  - pi4: N/A (default URL)

### Host, Build, and Target specification

If you are building a compiler to produce code for the machine it runs on (a native compiler), you normally do not need to specify any operands to `configure`; it will try to guess the type of machine you are on and use that as the build, host and target machines.

- `host`
  - The *host* machine is the system where you want to run the resulting compiler (normally the build machine).
  - Deb: `aarch64-linux-gnu`
  - pi4: `aarch64-linux-gnu`
  - otherwise long directory names `aarch64-unknown-linux-gnu`
- `build`
  - The *build* machine is the system which you are using.
  - Deb: `aarch64-linux-gnu`
  - pi4: N/A (building a a native compiler)
- `target`
  - The *target* machine is the system for which you want the compiler to generate code.
  - Deb: `aarch64-linux-gnu`
  - pi4: N/A (building a a native compiler)

### General options

Use options to override several configure time options for GCC. A list of supported options follows; ‘configure --help’ may list other options, but those not listed below may not work and should not normally be used.

- `enable-languages`
  - Specify that only a particular subset of compilers and their runtime libraries should be built. The `default` option includes `c`, `c++`, `fortran`, and `objc`.
  - Deb: `c,ada,c++,go,d,fortran,objc,obj-c++,m2`
  - pi4: `c++`
- `prefix`
  - Specify the top-level installation directory. Recommended way to install the tools into a directory other than the default `/usr/local`.
  - Deb: `/usr`
  - pi4: N/A (`/usr/local`)
- `exec-prefix`
  - Specify the top-level installation directory for architecture-dependent files. The default is `${prefix}`.
  - Deb: N/A (`/usr`)
  - pi4: N/A (`/usr/local`)
- `bindir`
  - Specify the installation directory for the executables called by users (such as `gcc` and `g++`). The default is `${exec-prefix}/bin`.
  - Deb: N/A (`/usr/bin`)
  - pi4: N/A (`/usr/local/bin`)
- `libdir`
  - Specify the installation directory for object code libraries and internal data files of GCC. The default is `${exec-prefix}/lib`.
  - Deb: `/usr/lib`
  - pi4: N/A (`/usr/local/lib`)
- `libexecdir`
  - Specify the installation directory for internal executables of GCC. The default is `${exec-prefix}/libexec`.
  - Deb: `/usr/lib`
  - pi4: N/A (`/usr/local/libexec`)
- `program-prefix`
  - Prepends prefix to the names of programs to install in `${bindir}`. For example, specifying `--program-prefix=foo-` would result in `gcc` being installed as `/usr/local/bin/foo-gcc`.
  - Deb: `aarch64-linux-gnu-`
  - pi4: N/A
- `program-suffix`
  - Appends suffix to the names of programs to install in `${bindir}`. For example, specifying `--program-suffix=-3.1` would result in `gcc` being installed as `/usr/local/bin/gcc-3.1`.
  - Deb: `-10`
  - pi4: N/A
- `with-gcc-major-version-only`
  - Specifies that GCC should use only the major number rather than `major.minor.patchlevel` in filesystem paths.
  - Deb: with
  - pi4: with
- `enable-shared`
  - Build shared versions of libraries, if shared libraries are supported on the target platform.
  - Deb: enable
  - pi4: enable
- `enable-multiarch`
  - Specify whether to enable or disable multiarch support. The default is to check for `glibc` start files in a multiarch location, and enable it if the files are found.
  - Deb: enable
  - pi4: N/A
- `enable-threads`
  - Specify that the target supports threads. This affects exception handling for C++.
  - Deb: `posix`
  - pi4: `posix`
- `enable-link-mutex`
  - When building GCC, use a mutex to avoid linking the compilers for multiple languages at the same time, to avoid thrashing on build systems with limited free memory.
  - Deb: enable
  - pi4: enable
- `enable-bootstrap`
  - For a native build, the default configuration is to perform a 3-stage bootstrap of the compiler when `make` is invoked, testing that GCC can compile itself correctly.`--disable-bootstrap`.
  - Deb: enable
  - pi4: enable
- `disable-libquadmath`
  - Specify that the GCC quad-precision math library should not be built. On some systems, the library is required to be linkable when building the Fortran front end.
  - Deb: disable
  - pi4: disable
- `disable-libquadmath-support`
  - Specify that the Fortran front end and libgfortran do not add support for libquadmath on systems supporting it.
  - Deb: disable
  - pi4: disable
- `enable-default-pie`
  - Enable Position Independent Executable by default.
  - Deb: enable
  - pi4: enable
- `disable-werror`
  - When you specify this option, it controls whether certain files in the compiler are built with `-Werror` in bootstrap stage2 and later.
  - Deb: disable
  - pi4: disable
- `enable-checking`
  - This option controls performing internal consistency checks in the compiler. It does not change the generated code, but adds error checking of the requested complexity. This slows down the compiler and may only work properly if you are building the compiler with GCC. `release` is a cheapest checks `assert,runtime`.
  - Deb: `release`
  - pi4: `release`
- `enable-nls`
  - Enables Native Language Support (NLS), which lets GCC output diagnostics in languages other than American English.
  - Deb: enable
  - pi4: disable
- `without-included-gettext`
  - If NLS is enabled, the `--with-included-gettext` option causes the build procedure to prefer its copy of GNU `gettext`.
  - Deb: without
  - pi4: N/A (as `nls` disabled)
- `enable-linker-build-id`
  - Tells GCC to pass `--build-id` option to the linker for all final links (links performed without the `-r` or `--relocatable` option), if the linker supports it.
  - Deb: enable
  - pi4: enable
- `enable-gnu-unique-object`
  - Tells GCC to use the `gnu_unique_object` relocation for C++ template static data members and inline function local statics.
  - Deb: enable
  - pi4: enable
- `with-system-zlib`
  - Use installed `libz`, a fast lossless compression algorithm.
  - Deb: with
  - pi4: with
- `enable-plugin`
  - enable plugin support
  - Deb: enable
  - pi4: enable
- `enable-clocale[=MODEL]`
  - use MODEL for target locale package, default=`auto`.
  - Deb: `gnu`
  - pi4: `gnu`
- `enable-libstdcxx-debug`
  - build extra debug library, default=`no`.
  - Deb: enable
  - pi4: enable
- `enable-libstdcxx-time[=KIND]`
  - use `KIND` for check type, default=`auto`.
  - Deb: `yes`
  - pi4: `yes`
- `with-default-libstdcxx-abi`
  - set the `std::string` ABI to use by default
  - Deb: `new`
  - pi4: `new`

### Objective-C-specific options

The following options apply to the build of the Objective-C runtime library.

- `enable-objc-gc`
  - Deb: `auto`

### D-specific options

The following options apply to the build of the D runtime library.

- `enable-libphobos-checking`
  - Deb: `release`
- `with-target-system-zlib`
  - Deb: `auto`

### `aarch64`-specific notes

- `enable-fix-cortex-a53-843419`
  - Enable a workaround for the Cortex-A53 erratum number 843419 by default (for all CPUs regardless of -mcpu option given)
  - Deb: enable
  - pi4: disable

For [Raspberry Pi 4B](https://www.raspberrypi.com/products/raspberry-pi-4-model-b/specifications/), the `fix-cortex-a53-843419` option should not be necessary because its CPU is Broadcom BCM2711, Quad core Cortex-A72 (ARM v8) 64-bit SoC.

### Options of GCC 12.2 build `configure`

```
$ cd obj-dir
$ src-dir/configure \
--host=aarch64-linux-gnu \
--enable-languages=c++ \
--with-gcc-major-version-only \
--enable-shared \
--enable-threads=posix \
--enable-link-mutex \
--enable-bootstrap \
--disable-libquadmath \
--disable-libquadmath-support \
--enable-default-pie \
--disable-werror \
--enable-checking=release \
--disable-nls \
--enable-linker-build-id \
--enable-gnu-unique-object \
--with-system-zlib \
--enable-plugin \
--enable-clocale=gnu \
--enable-libstdcxx-debug \
--enable-libstdcxx-time=yes \
--with-default-libstdcxx-abi=new \
--with-build-config=bootstrap-lto-lean
```

## Building

It is normal to have compiler warnings when compiling certain files. Unless you are a GCC developer, you can generally ignore these warnings unless they cause compilation to fail. Developers should attempt to fix any warnings encountered, however they can temporarily continue past warnings-as-errors by specifying the configure flag `--disable-werror`.

To build GCC:

```
% mkdir obj-dir
% cd obj-dir
% make -j5
```

Giving the `-j5` option to `make` to use five threads to build GCC. For [Raspberry Pi 4B](https://www.raspberrypi.com/products/raspberry-pi-4-model-b/specifications/), it should be five because its CPU is Broadcom BCM2711, Quad core Cortex-A72 (ARM v8) 64-bit SoC.

If your hard drive is slow, don't use `-jN`, otherwise `lock-and-run.sh` of GCC causes many file-locks, resulting in longer time to finish a build.

> [Building GCC, OS Dev org](https://wiki.osdev.org/Building_GCC): As the build can take a long time, it is recommended to make use of make's `-jN` option. This will allow make to use multiple threads to compile the programs, which will speed up things a LOT. Substitute `N` with a number; a good guideline is the number of core you CPU has, plus one.

With Raspberry Pi 4B, 8GB RAM, 32 GB memory disk:
- Started: 10:02
- End: 23:43
- Elapsed: 13 hours 41 minutes

### Building a native compiler

For a native build, the default configuration is to perform a 3-stage bootstrap of the compiler when `make` is invoked. This will build the entire GCC system and ensure that it compiles itself correctly. It can be disabled with the `--disable-bootstrap` parameter to *configure*, but bootstrapping is suggested because the compiler will be tested more completely and could also have better performance.

If you are short on disk space you might consider `make bootstrap-lean` instead. The sequence of compilation is the same described above, but object files from the stage1 and stage2 of the 3-stage bootstrap of the compiler are deleted as soon as they are no longer needed.

- `with-build-config`
  - The configuration option `bootstrap-lto-lean` lets the build process delete object files from the stage1 and stage2 of the 3-stage bootstrap of the compiler as soon as they are no longer needed.
  - Deb: `bootstrap-lto-lean`
  - pi4: `bootstrap-lto-lean`

`BUILD_CONFIG` can be used to bring in additional customization to the build. It can be set to a whitespace-separated list of names. For each such `NAME`, top-level `config/NAME.mk` will be included by the top-level `Makefile`, bringing in any settings it contains. The default `BUILD_CONFIG` can be set using the configure option `--with-build-config=NAME...`. Some examples of supported build configurations are:

- `bootstrap-lto`: Enables Link-Time Optimization for host tools during bootstrapping.
- `bootstrap-lto-lean`: This option is similar to `bootstrap-lto`, but is intended for faster build by only using LTO in the final bootstrap stage. With `make profiledbootstrap` the LTO frontend is trained only on generator files.

## Installing

Let's install GCC 12.2. Because we are installing it to `/usr/local`, `sudo` is required to run `make`:

```
% cd obj-dir
% sudo make install
```

Thanks to the default `PATH` setting, which puts `/usr/local/bin` before `usr/bin`:

```
$ which g++
/usr/local/bin/g++

$ echo $PATH
/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/local/games:/usr/games

$ g++ --version
g++ (GCC) 12.2.0
```

You still need to set LD_LIBRARY_PATH telling OS where GCC shared libraries are located. Edit `~/.bashrc` to append the following line:

```
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib64
```

### Upon Installation

| before                            | after
|-----------------------------------|-------
| /usr/local                        | /usr/local
| /usr/local/bin                    | /usr/local/bin
| /usr/local/etc                    | /usr/local/etc
| /usr/local/games                  | /usr/local/games
| /usr/local/include                | /usr/local/include
|                                   | /usr/local/include/c++
|                                   | /usr/local/include/c++/12
| /usr/local/lib                    | /usr/local/lib
| /usr/local/lib/pypy2.7            | /usr/local/lib/pypy2.7
| /usr/local/lib/python3.9          | /usr/local/lib/python3.9
|                                   | /usr/local/lib/gcc
|                                   | /usr/local/lib/gcc/aarch64-linux-gnu
|                                   | /usr/local/lib/gcc/aarch64-linux-gnu/12
|                                   | /usr/local/lib64
|                                   | /usr/local/libexec
|                                   | /usr/local/libexec/gcc
|                                   | /usr/local/libexec/gcc/aarch64-linux-gnu
|                                   | /usr/local/libexec/gcc/aarch64-linux-gnu/12
| /usr/local/sbin                   | /usr/local/sbin
| /usr/local/src                    | /usr/local/src
| /usr/local/share/sgml             | /usr/local/share/sgml
| /usr/local/share/fonts            | /usr/local/share/fonts
|                                   | /usr/local/share/gcc-12
| /usr/local/share/xml              | /usr/local/share/xml
| /usr/local/share/man              | /usr/local/share/man
|                                   | /usr/local/share/man/man7
|                                   | /usr/local/share/man/man1
|                                   | /usr/local/share/info
| /usr/local/share/ca-certificates  | /usr/local/share/ca-certificates
| /usr/local/share                  | /usr/local/share

```
$ ls -l /usr/local
total 40
drwxr-xr-x 2 root root 4096 Apr 10 23:49 bin
drwxr-xr-x 2 root root 4096 Feb 21 11:51 etc
drwxr-xr-x 2 root root 4096 Feb 21 11:51 games
drwxr-xr-x 3 root root 4096 Apr 10 23:50 include
drwxr-xr-x 5 root root 4096 Apr 10 23:47 lib
drwxr-xr-x 3 root root 4096 Apr 10 23:50 lib64
drwxr-xr-x 3 root root 4096 Apr 10 23:47 libexec
lrwxrwxrwx 1 root root    9 Feb 21 11:51 man -> share/man
drwxr-xr-x 2 root root 4096 Feb 21 11:51 sbin
drwxr-xr-x 9 root root 4096 Apr 10 23:50 share
drwxr-xr-x 2 root root 4096 Feb 21 11:51 src

$ ls -l /usr/local/bin
total 339772
-rwxr-xr-x 4 root root   5612904 Apr 10 23:47 aarch64-linux-gnu-c++
-rwxr-xr-x 4 root root   5612904 Apr 10 23:47 aarch64-linux-gnu-g++
-rwxr-xr-x 3 root root   5605944 Apr 10 23:49 aarch64-linux-gnu-gcc
-rwxr-xr-x 3 root root   5605944 Apr 10 23:49 aarch64-linux-gnu-gcc-12
-rwxr-xr-x 2 root root    114576 Apr 10 23:49 aarch64-linux-gnu-gcc-ar
-rwxr-xr-x 2 root root    114680 Apr 10 23:49 aarch64-linux-gnu-gcc-nm
-rwxr-xr-x 2 root root    114696 Apr 10 23:49 aarch64-linux-gnu-gcc-ranlib
-rwxr-xr-x 4 root root   5612904 Apr 10 23:47 c++
-rwxr-xr-x 1 root root   5608896 Apr 10 23:49 cpp
-rwxr-xr-x 4 root root   5612904 Apr 10 23:47 g++
-rwxr-xr-x 3 root root   5605944 Apr 10 23:49 gcc
-rwxr-xr-x 2 root root    114576 Apr 10 23:49 gcc-ar
-rwxr-xr-x 2 root root    114680 Apr 10 23:49 gcc-nm
-rwxr-xr-x 2 root root    114696 Apr 10 23:49 gcc-ranlib
-rwxr-xr-x 1 root root   4603208 Apr 10 23:49 gcov
-rwxr-xr-x 1 root root   2457560 Apr 10 23:49 gcov-dump
-rwxr-xr-x 1 root root   2623216 Apr 10 23:49 gcov-tool
-rwxr-xr-x 1 root root 292639120 Apr 10 23:47 lto-dump

$ ls -l /usr/local/include
total 4
drwxr-xr-x 3 root root 4096 Apr 10 23:50 c++

 $ ls -l /usr/local/lib
total 12
drwxr-xr-x 3 root root 4096 Apr 10 23:47 gcc
drwxr-xr-x 3 root root 4096 Feb 21 12:01 pypy2.7
drwxr-xr-x 3 root root 4096 Feb 21 11:54 python3.9

$ ls -l /usr/local/share/man
total 8
drwxr-xr-x 2 root root 4096 Apr 10 23:49 man1
drwxr-xr-x 2 root root 4096 Apr 10 23:49 man7
```

## What is bootstrap?

Reference: [Building GCC, OS Dev org](https://wiki.osdev.org/Building_GCC)

Compilers are upgraded through a process called *bootstrapping*. At first you have your old system compiler that produces slow code and doesn't support all the new language features. You then use this old system compiler to build the new version of the compiler, hoping that the old compiler is able to build the new compiler (it supports all needed features). This produces a new compiler that produces fast code and supports all the new features, however the compiler itself is slow, because it was compiled with a compiler that produces slow code. In addition, the new compiler may be buggy because your old compiler was buggy, or perhaps the new compiler release has a bug.

The next step is to use your new slow compiler that produces fast code, and then build the new compiler again. This produces a fast compiler that produces fast code. However, the first compiler we built could be buggy, and the compiler we just built using it may be defect. We need to verify the correctness of our new fast compiler that produces fast code.

To solve that problem, we build the compiler a third time. Once we have built the third compiler using the second compiler, it should produce the very same output as the first compiler building the second, as both times we are using compilers that produce fast code and use the same source code. The compiler build system will then verify that the second and third compilers are identical, which gives you confidence in the bootstrap. If the second and third compilers are not identical, the bootstrap failed and you have encountered a compiler bug. Bootstrapping takes three times as long as just building a regular compiler, but it makes sure your toolchain is stable.

The last thing to do is run the compiler test suite so you can verify that it works correctly.

