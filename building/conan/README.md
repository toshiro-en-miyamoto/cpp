# [Conan](https://conan.io/)

A software package manager for C and C++ developers.

## [Installing Conan 2.0](https://docs.conan.io/2/installation.html)

The preferred and strongly recommended way to install Conan is from PyPI, the Python Package Index, using the `pip` command.

Using Python virtual environment is strongly recommended. If not, it is possible that conan dependencies will conflict with previously existing dependencies, especially if you are using Python for other purposes.

```bash
$ python -m venv ~/venv/conan2
```

Running the [`venv` module](https://docs.python.org/3/library/venv.html) creates the target directory (`~/venv/conan2`) and places a `pyvenv.cfg` file in it with a home key pointing to the Python installation from which the command was run.

A virtual environment may be *activated* using a script in its binary directory. This will prepend that directory to your `PATH`, so that running `python` will invoke the environment’s Python interpreter and you can run installed scripts without having to use their full path.

```bash
$ source ~/venv/conan2/bin/activate
(conan2) ~ $ which pip
~/venv/conan2/bin/pip
(conan2) ~ $
(conan2) ~ $ deactivate
~ $
```

Now you can install Conan in the virtual environment:

```bash
(conan2) ~ $ pip install conan
(conan2) build $ which conan
~/venv/conan2/bin/conan
(conan2) build $ 
```

## [Conan Profiles](https://docs.conan.io/2/reference/config_files/profiles.html)

*Conan profiles* allow users to define a configuration set for things like the compiler, build configuration, architecture, shared or static libraries, etc. Conan, by default, will not try to detect a profile automatically, so we need to create one. 

Profiles can be created with the detect option in `conan profile` command, and edited later.

```bash
(conan2) ~ $ conan profile detect
Found gcc 10
gcc>=5, using the major as version
gcc C++ standard library: libstdc++11
Detected profile:
[settings]
arch=armv8
build_type=Release
compiler=gcc
compiler.cppstd=gnu14
compiler.libcxx=libstdc++11
compiler.version=10
os=Linux
Saving detected profile to ~/.conan2/profiles/default
```

Conan will always set the default C++ standard as the one that the detected compiler version uses by default. If you want to use a different C++ standard, you can edit the default profile file directly.

```bash
(conan2) ~ $ cd ~/.conan2/profiles
(conan2) profiles $ mv default default.org
(conan2) profiles $ sed s/gnu14/20/ < default.org > default
(conan2) profiles $ diff default.org default
5c5
< compiler.cppstd=gnu14
---
> compiler.cppstd=20

(conan2) profiles cat default
[settings]
arch=armv8
build_type=Release
compiler=gcc
compiler.cppstd=20
compiler.libcxx=libstdc++11
compiler.version=10
os=Linux
```