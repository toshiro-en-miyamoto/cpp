# C++20 Modules

## How to build projects using modules

### Simple Hello World

```c++
// hello.cppm
module;
#include <iostream>
export module hello;
export void say_hello() {
  std::cout << "Hello World!\n";
}

// main.cpp
import hello;
int main() {
  say_hello();
  return 0;
}
```

Then, on the command line, invoke `clang++` like:

```bash
~ % clang++ -std=c++20 hello.cppm --precompile -o hello.pcm
~ % clang++ -std=c++20 main.cpp -fmodule-file=Hello=hello.pcm hello.pcm -o hello.out
~ % ./hello.out
Hello World!
```

### Module and module unit

A module consists of one or more module units. A module unit is a special kind of translation unit. A module unit should almost always start with a module declaration. The syntax of the module declaration is:

```
[export] module module_name[:partition_name];
```


