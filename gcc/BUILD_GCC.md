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

## Configuration of GCC 10.2.1

For example, with Debian Bulleyes 10.2:

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

It turns out that `cpp` and `g++` have the same configuration:

<style type="text/css">
b { color: Khaki;
    font-family: "Lucida Console", "Courier New", monospace;
    font-weight: normal;
}
</style>

If you will be distributing binary versions of GCC, with modifications to the source code, you should use the *distributor options* to make clear that your version contains modifications.

<table>
<tr><th colspan="3">Distributor options</th></tr>

<tr><td colspan="3"><b>with-pkgversion</b></td></tr>
<tr><td></td><td colspan="2">
This version string will be included in the output of <b>gcc --version</b>. The default value is <b>GCC</b>.</td></tr>
<tr><td></td><td>Deb</td><td><b>Debian 10.2.1-6</b></td></tr>
<tr><td></td><td>pi4</td><td>TBD</td></tr>

<tr><td colspan="3"><b>with-bugurl</b></td></tr>
<tr><td></td><td colspan="2">
Specify the URL that users should visit if they wish to report a bug. The default value refers to the FSF’s GCC bug tracker.</td></tr>
<tr><td></td><td>Deb</td><td><b>file:///usr/share/doc/gcc-10/README.Bugs</b></td></tr>
<tr><td></td><td>pi4</td><td>N/A (default URL)</td></tr>

</table>

Use options to override several configure time options for GCC. A list of supported options follows; ‘configure --help’ may list other options, but those not listed below may not work and should not normally be used.

<table>
<tr><th colspan="3">General options</th></tr>

<tr><td colspan="3"><b></b></td></tr>
<tr><td></td><td colspan="2">
</td></tr>
<tr><td></td><td>Deb</td><td><b></b></td></tr>
<tr><td></td><td>pi4</td><td></td></tr>

<tr><td colspan="3"><b></b></td></tr>
<tr><td></td><td colspan="2">
</td></tr>
<tr><td></td><td>Deb</td><td><b></b></td></tr>
<tr><td></td><td>pi4</td><td></td></tr>

<tr><td colspan="3"><b></b></td></tr>
<tr><td></td><td colspan="2">
</td></tr>
<tr><td></td><td>Deb</td><td><b></b></td></tr>
<tr><td></td><td>pi4</td><td></td></tr>

<tr><td colspan="3"><b>enable-languages</b></td></tr>
<tr><td></td><td colspan="2">
Specify that only a particular subset of compilers and their runtime libraries should be built. The <b>default</b> option includes <b>c</b>, <b>c++</b>, <b>fortran</b>, and <b>objc</b>.</td></tr>
<tr><td></td><td>Deb</td><td><b>c,ada,c++,go,d,fortran,objc,obj-c++,m2</b></td></tr>
<tr><td></td><td>pi4</td><td><b>c++</b></td></tr>

<tr><td colspan="3"><b>prefix</b></td></tr>
<tr><td></td><td colspan="2">
Specify the toplevel installation directory. Recommended way to install the tools into a directory other than the default <b>/usr/local</b>.</td></tr>
<tr><td></td><td>Deb</td><td><b>/usr</b></td></tr>
<tr><td></td><td>pi4</td><td>N/A (<b>/usr/local</b>)</td></tr>

<tr><td colspan="3"><b>exec-prefix</b></td></tr>
<tr><td></td><td colspan="2">
Specify the toplevel installation directory for architecture-dependent files. The default is <b>${prefix}</b>.</td></tr>
<tr><td></td><td>Deb</td><td>N/A (<b>/usr</b>)</td></tr>
<tr><td></td><td>pi4</td><td>N/A (<b>/usr/local</b>)</td></tr>

<tr><td colspan="3"><b>bindir</b></td></tr>
<tr><td></td><td colspan="2">
Specify the installation directory for the executables called by users (such as <b>gcc</b> and <b>g++</b>). The default is <b>${exec-prefix}/bin</b>.</td></tr>
<tr><td></td><td>Deb</td><td>N/A (<b>/usr/bin</b><)/td></tr>
<tr><td></td><td>pi4</td><td>N/A (<b>/usr/local/bin</b>)</td></tr>

<tr><td colspan="3"><b>libdir</b></td></tr>
<tr><td></td><td colspan="2">
Specify the installation directory for object code libraries and internal data files of GCC. The default is <b>${exec-prefix}/lib</b>.</td></tr>
<tr><td></td><td>Deb</td><td><b>/usr/lib</b></td></tr>
<tr><td></td><td>pi4</td><td>N/A (<b>/usr/local/lib</b>)</td></tr>

<tr><td colspan="3"><b>libexecdir</b></td></tr>
<tr><td></td><td colspan="2">
Specify the installation directory for internal executables of GCC. The default is <b>${exec-prefix}/libexec</b>.</td></tr>
<tr><td></td><td>Deb</td><td><b>/usr/lib</b></td></tr>
<tr><td></td><td>pi4</td><td>N/A (<b>/usr/local/libexec</b>)</td></tr>

<tr><td colspan="3"><b>program-prefix</b></td></tr>
<tr><td></td><td colspan="2">
Prepends prefix to the names of programs to install in <b>${bindir}</b>. For example, specifying <b>--program-prefix=foo-</b> would result in <b>gcc</b> being installed as <b>/usr/local/bin/foo-gcc</b>.</td></tr>
<tr><td></td><td>Deb</td><td><b>aarch64-linux-gnu-</b></td></tr>
<tr><td></td><td>pi4</td><td>TBD</td></tr>

<tr><td colspan="3"><b>program-suffix</b></td></tr>
<tr><td></td><td colspan="2">
Appends suffix to the names of programs to install in <b>${bindir}</b>. For example, specifying <b>--program-suffix=-3.1</b> would result in <i>gcc</i> being installed as <b>/usr/local/bin/gcc-3.1</b>.</td></tr>
<tr><td></td><td>Deb</td><td><b>-10</b></td></tr>
<tr><td></td><td>pi4</td><td>TBD</td></tr>

<tr><td colspan="3"><b>with-gcc-major-version-only</b></td></tr>
<tr><td></td><td colspan="2">
Specifies that GCC should use only the major number rather than <b>major.minor.patchlevel</b> in filesystem paths.</td></tr>
<tr><td></td><td>Deb</td><td><b>with</b></td></tr>
<tr><td></td><td>pi4</td><td>TBD</td></tr>

<tr><td colspan="3"><b>enable-shared</b></td></tr>
<tr><td></td><td colspan="2">
Build shared versions of libraries, if shared libraries are supported on the target platform.</td></tr>
<tr><td></td><td>Deb</td><td><b>enable</b></td></tr>
<tr><td></td><td>pi4</td><td><b>enable</b></td></tr>

<tr><td colspan="3"><b>enable-multiarch</b></td></tr>
<tr><td></td><td colspan="2">
Specify whether to enable or disable multiarch support. The default is to check for <b>glibc</b> start files in a multiarch location, and enable it if the files are found.</td></tr>
<tr><td></td><td>Deb</td><td><b>enable</b></td></tr>
<tr><td></td><td>pi4</td><td>TBD</td></tr>

<tr><td colspan="3"><b>enable-threads</b></td></tr>
<tr><td></td><td colspan="2">
Specify that the target supports threads. This affects exception handling for C++.</td></tr>
<tr><td></td><td>Deb</td><td><b>posix</b></td></tr>
<tr><td></td><td>pi4</td><td><b>posix</b></td></tr>

<tr><td colspan="3"><b>enable-link-mutex</b></td></tr>
<tr><td></td><td colspan="2">
When building GCC, use a mutex to avoid linking the compilers for multiple languages at the same time, to avoid thrashing on build systems with limited free memory.</td></tr>
<tr><td></td><td>Deb</td><td><b>enable</b></td></tr>
<tr><td></td><td>pi4</td><td><b>enable</b></td></tr>

<tr><td colspan="3"><b>enable-bootstrap</b></td></tr>
<tr><td></td><td colspan="2">
For a native build, the default configuration is to perform a 3-stage bootstrap of the compiler when <b>make</b> is invoked, testing that GCC can compile itself correctly. If you want to disable this process, you can configure with <b>--disable-bootstrap</b>.</td></tr>
<tr><td></td><td>Deb</td><td><b>enable</b></td></tr>
<tr><td></td><td>pi4</td><td><b>TBD</b></td></tr>

<tr><td colspan="3"><b></b>disable-libquadmath</td></tr>
<tr><td></td><td colspan="2">
Specify that the GCC quad-precision math library should not be built. On some systems, the library is required to be linkable when building the Fortran front end.</td></tr>
<tr><td></td><td>Deb</td><td><b>disable</b></td></tr>
<tr><td></td><td>pi4</td><td><b>disable</b></td></tr>

<tr><td colspan="3"><b>disable-libquadmath-support</b></td></tr>
<tr><td></td><td colspan="2">
Specify that the Fortran front end and libgfortran do not add support for libquadmath on systems supporting it.</td></tr>
<tr><td></td><td>Deb</td><td><b>disable</b></td></tr>
<tr><td></td><td>pi4</td><td><b>disable</b></td></tr>

<tr><td colspan="3"><b>enable-default-pie</b></td></tr>
<tr><td></td><td colspan="2">
Turn on -fPIE and -pie by default.</td></tr>
<tr><td></td><td>Deb</td><td><b>enable</b></td></tr>
<tr><td></td><td>pi4</td><td>TBD</td></tr>

<tr><td colspan="3"><b>disable-werror</b></td></tr>
<tr><td></td><td colspan="2">
When you specify this option, it controls whether certain files in the compiler are built with <b>-Werror</b> in bootstrap stage2 and later.</td></tr>
<tr><td></td><td>Deb</td><td><b>disable</b></td></tr>
<tr><td></td><td>pi4</td><td><b>disable</b></td></tr>

<tr><td colspan="3"><b>enable-checking</b></td></tr>
<tr><td></td><td colspan="2">
This option controls performing internal consistency checks in the compiler. It does not change the generated code, but adds error checking of the requested complexity. This slows down the compiler and may only work properly if you are building the compiler with GCC. <b>release</b> is a cheapest checks <b>assert,runtime</b>.</td></tr>
<tr><td></td><td>Deb</td><td><b>release</b></td></tr>
<tr><td></td><td>pi4</td><td><b>release</b></td></tr>

<tr><td colspan="3"><b>enable-nls</b></td></tr>
<tr><td></td><td colspan="2">
The <b>--enable-nls</b> option enables Native Language Support (NLS), which lets GCC output diagnostics in languages other than American English.</td></tr>
<tr><td></td><td>Deb</td><td><b>enable</b></td></tr>
<tr><td></td><td>pi4</td><td><b>disable</b></td></tr>

<tr><td colspan="3"><b>without-included-gettext</b></td></tr>
<tr><td></td><td colspan="2">
</td></tr>
<tr><td></td><td>Deb</td><td><b>without</b></td></tr>
<tr><td></td><td>pi4</td><td><b>without</b></td></tr>

<tr><td colspan="3"><b>enable-linker-build-id</b></td></tr>
<tr><td></td><td colspan="2">
Tells GCC to pass <b>--build-id</b> option to the linker for all final links (links performed without the <b>-r</b> or <b>--relocatable</b> option), if the linker supports it.</td></tr>
<tr><td></td><td>Deb</td><td><b>enable</b></td></tr>
<tr><td></td><td>pi4</td><td><b>enable</b></td></tr>

<tr><td colspan="3"><b>enable-gnu-unique-object</b></td></tr>
<tr><td></td><td colspan="2">
Tells GCC to use the <b>gnu_unique_object</b> relocation for C++ template static data members and inline function local statics.</td></tr>
<tr><td></td><td>Deb</td><td><b>enable</b></td></tr>
<tr><td></td><td>pi4</td><td><b>enable</b></td></tr>

</table>

The following options apply to the build of the Objective-C runtime library.

<table>
<tr><th colspan="3">Object-C-specific options</th></tr>

<tr><td colspan="3"><b>enable-objc-gc</b></td></tr>
<tr><td></td><td>Deb</td><td><b>auto</b></td></tr>
<tr><td></td><td>pi4</td><td>N/A</td></tr>

</table>

The following options apply to the build of the D runtime library.

<table>
<tr><th colspan="3">D-specific options</th></tr>

<tr><td colspan="3"><b>enable-libphobos-checking</b></td></tr>
<tr><td></td><td>Deb</td><td><b>release</b></td></tr>
<tr><td></td><td>pi4</td><td>N/A</td></tr>

<tr><td colspan="3"><b>with-target-system-zlib</b></td></tr>
<tr><td></td><td>Deb</td><td><b>auto</b></td></tr>
<tr><td></td><td>pi4</td><td>N/A</td></tr>

<tr><td colspan="3"><b></b></td></tr>
<tr><td></td><td colspan="2">
</td></tr>
<tr><td></td><td>Deb</td><td><b></b></td></tr>
<tr><td></td><td>pi4</td><td></td></tr>

</table>

| General option        | value (blank: enabled)
|-----------------------|---------------------
| --build                           | aarch64-linux-gnu
| --host                            | aarch64-linux-gnu
| --target                          | aarch64-linux-gnu
| --enable-clocale                  | gnu
| --enable-fix-cortex-a53-843419    |
| --enable-libstdcxx-debug          |*1
| --enable-libstdcxx-time           | yes
| --enable-plugin                   |
| --with-build-config               | bootstrap-lto-lean
| --with-default-libstdcxx-abi      | new
| --with-system-zlib                |

## Prerequisites

Reference: [General prerequisites for GCC](https://gcc.gnu.org/install/prerequisites.html)

Required compilers include:

- C++11 compiler
- C standard library and headers

Required tools include:

- `bash` (`zsh` will not work)
- `awk`
- `gzip` 1.2.4 (or later) or `bzip2` 1.0.2 (or later)
- `make`
- `tar`
- `python3`

Debian Bullseye 10.2 installs the tools by default.

| pack name  | ver   | description
|------------|-------|-------------
| `bash`     | 5.1.2 | GNU Bourne Again SHell
| `gawk`     | 5.1.0 | GNU awk
| `gzip`     | 1.10  | GNU compression utilities
| `make`     | 4.3   | utility for directing compilation
| `tar`      | 1.34  | GNU version of the tar archiving utility
| `python3`  | 3.9.2 | interactive high-level object-oriented language

Required libraries include:

- GMP version 4.3.2 (or later)
- MPFR Library version 3.1.0 (or later)
- MPC Library version 1.0.1 (or later)
- `isl` Library version 0.15 or later
- `zstd` Library

Debian Bullseye 10.2 installs the libraries by default.

| pack name  | ver   | description
|------------|-------|-------------
| `libgmp10` | 6.2.1 | Multiprecision arithmetic library
| `libmpfr6` | 4.1.0 | multiple precision floating-point computation
| `libmpc3`  | 1.2.0 | multiple precision complex floating-point library
| `libisl23` | 0.23  | manipulating sets and relations of integer points
| `libzstd1` | 1.4.8 | fast lossless compression algorithm

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
