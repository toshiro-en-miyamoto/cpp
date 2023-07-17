# [`conan list`](https://docs.conan.io/2/reference/commands/list.html)

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
