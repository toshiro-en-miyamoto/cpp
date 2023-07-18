# `conan install`

Install the requirements specified in a recipe (`conanfile.py` or `conanfile.txt`).

```bash
(conan2) ~ $ conan install path
```

`path`: Path to a folder containing a recipe (`conanfile.py` or `conanfile.txt`) or to a recipe file. e.g., `./my_project/conanfile.txt`.

1. If any requirement is not found in the local cache, it will retrieve the recipe from a remote, looking for it sequentially in the configured remotes.
2. When the recipes have been downloaded it will try to download a binary package matching the specified settings, only from the remote from which the recipe was retrieved.
3. If no binary package is found, it can be built from sources using the `--build` option.

When the package is installed, Conan will create the necessary files as requested by the *generators*, so build systems and other tools can locate the locally installed dependencies.

## `--requires` option

The other possibility is to not have a `conanfile` at all, and define the requirements to be installed directly in the command line:

```bash
(conan2) ~ $ conan install --requires=zlib/1.2.13

(conan2) ~ $ conan install --requires=zlib/1.2.13 --requires=bzip2/1.0.8

(conan2) ~ $ conan install --tool-requires=cmake/3.23.5 --tool-requires=ninja/1.11.0

(conan2) ~ $ conan install --requires=zlib/1.2.13 --tool-requires=ninja/1.11.0
```

## Profiles, `settings`, `options`, `conf`

There are several arguments that are used to define the effective profiles:

- By default the arguments refer to the `host` context, so `--settings` (or `-s` for short) is totally equivalent to `--settings:host` (or `-s:h`).
- Also, by default, the `conan install` command will use the default profile both for the `build` and `host` context. That means that if a profile with the `default` name has not been created, it will error.

Multiple definitions of profiles can be passed as arguments, and they will compound from left to right (right has the highest priority) - `prof3` in this case:

```bash
(conan2) ~ $ conan install . -pr=prof1 -pr=prof2 -pr=prof3
```

If values for any of `settings`, `options` and `conf` are provided in the command line, they create a profile that is composed with the other provided profiles (`-pr`, or the `default` one if not specified), with higher priority, not matter what the order of arguments is.

```bash
(conan2) ~ $ conan install . -pr=prof1 -s build_type=Debug
```

## Generators and deployers

Generators are intended to create files for the build systems to locate the dependencies. The `-generator` (or `-g` for short) allows to define in the command line the different built-in generators to be used:

```bash
(conan2) ~ $ conan install --requires=zlib/1.2.13 -g CMakeDeps -g CMakeToolchain
```

The `deployer` main use case is to copy files from the Conan cache to user space, and performing any other custom operations over the dependency graph, like collecting licenses, generating reports, deploying binaries to the system, etc. 

```bash
(conan2) ~ $ conan install . --deployer=full_deploy
```

There are 2 built-in deployers:

- `full_deploy` does a complete copy of the dependencies binaries in the local folder, with a minimal folder structure to avoid conflicts between files and artifacts of different packages
- `direct_deploy` does a copy of only the immediate direct dependencies, but does not include the transitive dependencies.

## Arguments

| category  | argument  | description
|-----------|-----------|-------------
| `conanfile.py` | **reference** |
|| `--name`             | a package name
|| `--version`          | a package version
|| `--user`             | a user name
|| `--channel`          | a channel name
|| **requirements** |
|| `--requires`         | regular dependencies in the host
|| `--tools-requires`   | a build tool like CMake
|| **binary model** |
|| `--profile`          | apply the specified profile
|| `--settings`         | settings to build the package
|| `--options`          | options values
|| `--conf`             | configuration to build the package
|| **build** |
|| `--build`            | specify which packages to build
|| `--generator`        | generator to use
|| `--deployer`         | deploy using the provided deployer
|| **folders and layout** |
|| `--output-folder`    | folder for generated and build files
|| `--deployer-folder`  | deployer output folder
| `conan install` | **options** |
|| `--format`           | the output format: `json`
|| `--remote`           | look in the specified remote
|| `--no-remote`        | resolve exclusively in the cache
|| `--update`           | try the local cache up-to-date
|| **advanced** |
|| `--build-require`      | the reference is a build-require
|| `--lockfile`           | 
|| `--lockfile-partial`   | 
|| `--lockfile-out`       | 
|| `--lockfile-package`   | 
|| `--lockfile-overrides` | 
