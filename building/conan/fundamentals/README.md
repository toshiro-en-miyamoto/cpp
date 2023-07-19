# Conan fundamentals

- Recipes
  - `conanfile.py`
  - `conanfile.txt`
- Recipe tools
  - `conan.tools.build`
  - `conan.tools.cmake`
  - `conan.tools.apple`
  - `conan.tools.microsoft`
  - `conan.tools.android`
  - `conan.tools.scm`
- Configuration
  - `global.conf`
  - built-in conf
  - profiles
  - `settings.yml`
  - `remotes.json`
  - `source_credentials.json`

[A **package** is defined by a `conanfile.py`.](https://docs.conan.io/2/introduction.html) This is a file that defines the package’s dependencies, sources, how to build the binaries from sources, etc. One package `conanfile.py` recipe can generate any arbitrary number of binaries, one for each different platform and **configuration**: operating system, architecture, compiler, build type, etc.

[The `conanfile.txt` file](https://docs.conan.io/2/reference/conanfile_txt.html) is a simplified version of `conanfile.py`, aimed at simple consumption of dependencies, but it cannot be used to create a package. Also, it is not necessary to have a `conanfile.txt` for consuming dependencies, a `conanfile.py` is perfectly suited for simple consumption of dependencies.

[The easiest way to install the Zlib library](https://docs.conan.io/2/tutorial/consuming_packages/build_simple_cmake_project.html) and find it from our project with Conan is using a `conanfile.txt` file.

```
[requires]
zlib/1.2.11

[generators]
CMakeDeps
CMakeToolchain
```

- `[requires]` section is where we declare the libraries we want to use in the project.
- `[generators]` section tells Conan to generate the files that the compilers or build systems will use to find the dependencies and build the project.
