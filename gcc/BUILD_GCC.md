# Building GCC

## The filesystem hierarchy

Reference: [HIER(7) the filesystem hierarchy](https://www.man7.org/linux/man-pages/man7/hier.7.html)

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

For example, with Debian Bulleyes:

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

## Prerequisites

Reference: [General prerequisites for GCC](https://gcc.gnu.org/install/prerequisites.html)

- C++11 compiler
- C standard library and headers

Tools:

- `bash` (`zsh` will not work)
- `awk`
- `gzip` 1.2.4 (or later) or `bzip2` 1.0.2 (or later)
- `make`
- `tar`
- Python3

Libraries:

- GNU Multiple Precision Library (GMP) version 4.3.2 (or later)
- MPFR Library version 3.1.0 (or later)
- MPC Library version 1.0.1 (or later)
- `isl` Library version 0.15 or later
- `zstd` Library

## Configuration

[Installing GCC: Configuration](https://gcc.gnu.org/install/configure.html)

- `srcdir`: the toplevel source directory
- `objdir`: the toplevel build/object directory

Recommending `srcdir != objdir`. Building where `objdir` is a subdirectory of `srcdir` is unsupported.

`cc` or `gcc` must be in your path.

To configure GCC:

```
% mkdir objdir
% cd objdir
% srcdir/configure [options] [target]
```

If you will be distributing binary versions of GCC, with modifications to the source code, you should use the options to make clear that your version contains modifications.

### Host, Build, and Target specification

If you are building a compiler to produce code for the machine it runs on (a native compiler), you normally do not need to specify any operands to `configure`; it will try to guess the type of machine you are on and use that as the build, host and target machines.

- The *build* machine is the system which you are using,
- the *host* machine is the system where you want to run the resulting compiler (normally the build machine), and
- the *target* machine is the system for which you want the compiler to generate code.

### Options specification

#### `--prefix=dirname`:

- Specify the top level installation directory. This is the recommended way to install the tools into a directory other than the default. The top level installation directory defaults to `/usr/local`.
- We highly recommend against *dirname* being the same or a subdirectory of *objdir* or vice versa. If specifying a directory beneath a user’s home directory tree, some shells will not expand *dirname* correctly.

Default directories:

- `/usr/local`: the top level installation directory
- `/usr/local/bin`: the executables called by users
- `/usr/local/lib`: object code libraries
- `/usr/local/lib`: the shared `libgcc` library
- `/usr/local/libexec`: internal executables of GCC
- `/usr/local/share`: arch-independent data files
- `/usr/local/share/info`: documentation in `info` format
- `/usr/local/share/doc`: documentation other than `info`
- `/usr/local/share/man`: manual pages

#### Include path

The purpose of `--prefix` (a.k.a GCC-prefix) is to specify where to install GCC. GCC installs its own header files in another directory which is based on the `--prefix` value.

The local header files in `/usr/local/include`—if you put any in that directory—are not part of GCC. They are part of other programs—perhaps many others. ()

`--with-local-prefix=dirname` specifies the installation directory for *local-prefix* include files. The default is `/usr/local` regardless of the value of `--prefix`, which lets the compiler to search directory `/usr/local/include`.


#### Program name

`--program-prefix=prefix`:

- GCC supports some transformations of the names of its programs when installing them. This option prepends `prefix` to the names of programs to install in `bin` (see above).
- For example, specifying `--program-prefix=foo-` would result in ‘gcc’ being installed as `/usr/local/bin/foo-gcc`.

`--program-suffix=suffix`:

Appends `suffix` to the names of programs to install in `bin` (see above). For example, specifying `--program-suffix=-3.1` would result in `gcc` being installed as `/usr/local/bin/gcc-3.1`.

#### Languages

`--enable-languages=lang1,lang2,…`:

Specify that only a particular subset of compilers and their runtime libraries should be built: `all`, `default`, and,

- default: `c`, `c++`, `fortran`, `lto`, `objc`
- not default: `ada`, `d`, `go`, `jit`, `m2`, `obj-c++`

The following options build `c++` only:

```
--enable-languages=c++
```

Support for link-time optimization (LTO) is enabled by default.

#### National Language Support (NLS)

NLS is enabled by default, which lets GCC output diagnostics in languages other than American English.

```
--disable-nls
```
