# Conan

[Conan](https://conan.io/) is a dependency and package manager for C and C++ languages.

- is free and open-source, works in all platforms ( Windows, Linux, OSX, FreeBSD, Solaris, etc.)
- can be used to develop for all targets including embedded, mobile (iOS, Android), and bare metal
- integrates with all build systems like CMake, Visual Studio (MSBuild), Makefiles, SCons, etc., including proprietary ones

Conan is specifically designed and optimized for accelerating the development and Continuous Integration of C and C++ projects. With full binary management, it can create and reuse

- any number of different binaries (for different configurations like architectures, compiler versions, etc.)
- for any number of different versions of a package, using exactly the same process in all platforms.

## Installing Conan 2.0

The preferred and strongly recommended way to [install Conan](https://docs.conan.io/2/installation.html) is from PyPI, the Python Package Index, using the `pip` command.

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

## Conan Profiles

[*Conan profiles*](https://docs.conan.io/2/reference/config_files/profiles.html) allow users to define a configuration set for things like the compiler, build configuration, architecture, shared or static libraries, etc. Conan, by default, will not try to detect a profile automatically, so we need to create one. 

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

For more details, [here](./fundamentals/README.md)

> [Best practices](https://docs.conan.io/2/reference/commands/profile.html): It is not recommended to use `conan profile detect` in production. To guarantee reproducibility, it is recommended to define your own profiles, store them in a git repo or in a zip in a server, and distribute it to your team and CI machines with `conan config install`, together with other configuration like custom settings, custom remotes definition, etc.
