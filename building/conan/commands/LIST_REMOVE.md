# List and Remove

## [`conan list`](https://docs.conan.io/2/reference/commands/list.html)

The `conan list` command can list recipes and packages from the local cache, from the specified remotes or from both.

To list all references on local cache:

```bash
(conan2) ~ $ conan list "*"
Local Cache
  nlohmann_json
    nlohmann_json/3.11.2
  zlib
    zlib/1.2.13
```

You can list:

| object                                | reference pattern
|---------------------------------------|-------------------
| Recipe references                     | `recipe-name/version`
| Recipe revisions                      | `recipe-name/version#recipe-revision`
| Package IDs and their configurations  | `recipe-name/version#recipe-revision:package-ids`
| Package revisions                     | `recipe-name/version#recipe-revision:package-ids#package-revision`

To list all versions of a reference on Conan Center:

```bash
(conan2) ~ $ conan list zlib --remote conancenter
conancenter
  zlib
    zlib/1.2.8
    zlib/1.2.11
    zlib/1.2.12
    zlib/1.2.13
```

> By default Conan will be listing all versions.

To list all recipe revisions:

```bash
(conan2) ~ $ conan list "zlib/1.2.13#*" --remote conancenter
conancenter
  zlib
    zlib/1.2.13
      revisions
        40e9a7e1.... (2022-10-14 12:06:41 UTC)
        647c91ed.... (2022-10-19 09:19:10 UTC)
        13c96f53.... (2022-11-02 13:46:53 UTC)
        e377bee6.... (2023-04-27 12:11:24 UTC)
```

To list all package IDs for latest recipe revision:

```bash
(conan2) ~ $ conan list "zlib/1.2.13:*"
Local Cache
  zlib
    zlib/1.2.13
      revisions
        e377bee6.... (2023-04-27 12:11:24 UTC)
          packages
            0090a2a7....
              info
                settings
                  arch: arm v8
                  build_type: Release
                  compiler: gcc
                  compiler.version: 10
                  os: Linux
                options
                  fPIC: True
                  shared: False
```

Here the `#latest` for the recipe revision is implicit, i.e., that pattern is equivalent to `zlib/1.2.13#latest:*`.

> By default Conan will be listing the latest revisions.

## [`conan remove`](https://docs.conan.io/2/reference/commands/remove.html)

The `conan remove`` command removes recipes and packages from the local cache or from a specified remote.

It has 2 possible and mutually exclusive inputs:

- The `conan remove <pattern>` pattern-based matching of recipes.
- The `conan remove --list=<pkg-list>` that will remove the artifacts specified in the `pkg-list` json file.

To remove recipes and their associated package binaries from the local cache:

```bash
(conan2) ~ $ conan remove "*"
# Removes everything from the cache

(conan2) ~ $ conan remove "zlib/*"
# Remove all possible versions of zlib, including all recipes, revisions and packages

(conan2) ~ $ conan remove zlib/1.2.11
# Remove zlib/1.2.11, all its revisions and package binaries. Leave other zlib versions

(conan2) ~ $ conan remove "zlib/[<1.2.13]"
# Remove zlib/1.2.11 and zlib/1.2.12, all its revisions and package binaries.
```

You can also use the keyword `latest` or `!latest` in the revision part of the pattern.

```bash
(conan2) ~ $ conan remove zlib/1.2.11#latest
# Remove zlib/1.2.11, only its latest recipe revision and binaries of that revision
# Leave the other zlib/1.2.11 revisions intact

(conan2) ~ $ conan remove zlib/1.2.11#!latest
# Remove all the recipe revisions from zlib/1.2.11 but the latest one
# Leave the latest zlib/1.2.11 revision intact

(conan2) ~ $ conan remove zlib/1.2.11#<revision>
# Remove zlib/1.2.11, only its exact <revision> and binaries of that revision
# Leave the other zlib/1.2.11 revisions intact
```

To remove only package binaries, but leaving the recipes, it is necessary to specify the pattern including the `:` separator of the `package_id`:

```bash
(conan2) ~ $ conan remove "zlib/1.2.11:*"
# Removes all the zlib/1.2.11 package binaries from all the recipe revisions

(conan2) ~ $ conan remove "zlib/*:*"
# Removes all the binaries from all the recipe revisions from all zlib versions

(conan2) ~ $ conan remove "zlib/1.2.11#latest:*"
# Removes all the zlib/1.2.11 package binaries only from the latest zlib/1.2.11 recipe revision

(conan2) ~ $ conan remove "zlib/1.2.11#!latest:*"
# Removes all the zlib/1.2.11 package binaries from all the recipe revisions but the latest one

(conan2) ~ $ conan remove zlib/1.2.11:<package_id>
# Removes the package binary <package_id> from all the zlib/1.2.11 recipe revisions

(conan2) ~ $ conan remove zlib/1.2.11:#latest<package_id>#latest
# Removes only the latest package revision of the binary identified with <package_id>
# from the latest recipe revision of zlib/1.2.11
# WARNING: Recall that having more than 1 package revision is a smell and shouldn't happen
# in normal situations
```

Note that you can filter which packages will be removed using the `--package-query` argument:

```bash
(conan2) ~ $ conan remove zlib/1.2.11:* -p compiler=clang
# Removes all the zlib/1.2.11 packages built with Clang compiler
```

You can query packages by both their settings and options, including custom ones. To query for options you need to explicitly add the options. prefix, so that `-p options.shared=False` will work but `-p shared=False` won’t.
