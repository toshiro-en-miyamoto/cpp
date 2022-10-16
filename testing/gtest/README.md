## VS Code properties

With Gtest, your test codes must `#include <gtest/gtest.h>`. You might want to let VS Code aware of the location of the file by adding the following value into the `includePath` attribte in `.vscode/c_cpp_properties.json`.

```json
{
    "configurations": [
        {
            "name": "Linux",
            "includePath": [
                "${workspaceFolder}/**",
                "bazel-cpp/external/com_google_googletest/googletest/include/"
            ],
            "defines": [],
            "compilerPath": "/usr/bin/gcc",
            "cStandard": "c17",
            "cppStandard": "c++20"
        }
    ],
    "version": 4
}```
