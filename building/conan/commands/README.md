# [Conan 2.0 Commands](https://docs.conan.io/2/reference/commands.html)

Consumer commands:

| command             | description
|---------------------|---------------
| `conan list`        | List recipes, revisions and packages in the local cache or in remotes [(-> here)](./LIST.md)
| `conan cache`       | Return the path of recipes and packages in the cache
| `conan config`      | Manage Conan configuration (remotes, settings, plugins, etc)
| `conan graph`       | Obtain information about the dependency graph without fetching binaries
| `conan inspect`     | Inspect a conanfile.py to return the public fields
| `conan install`     | Install dependencies
| `conan lock`        | Create and manage lockfiles
| `conan profile`     | Display and manage profile files
| `conan remove`      | Remove packages from the local cache or from remotes
| `conan remote`      | Add, remove, login/logout and manage remote server
| `conan search`      | Search packages matching a name
| `conan version`     | Give information about the Conan client version

Creator commands:

| command             | description
|---------------------|---------------
| `conan build`       | Install package and call its build method
| `conan create`      | Create a package from a recipe
| `conan download`    | Download (without install) a single conan package from a remote server.
| `conan editable`    | Allows working with a package in user folder
| `conan export`      | Export a recipe to the Conan package cache
| `conan export-pkg`  | Create a package directly from pre-compiled binaries
| `conan new`         | Create a new recipe from a predefined template
| `conan source`      | Calls the source() method
| `conan test`        | Test a package
| `conan upload`      | Upload packages from the local cache to a specified remote

