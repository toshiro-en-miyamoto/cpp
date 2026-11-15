# C++20 Modules

- [C++Ref] [Modules (since C++20), cppreference](https://www.cppreference.com/cpp/language/modules)
- [Clang] [Standard C++ Module, Clang](https://clang.llvm.org/docs/StandardCPlusPlusModules.html#how-to-build-projects-using-modules)

## Modules [C++Ref]

```c++
// helloworld.cpp
export module helloworld;  // module declaration
import <iostream>;         // import declaration
export void hello() ()     // export declaration
{
  std::cout << "Hello world!\n";
}

// main.cpp
import helloworld;         // import declaration
int main()
{
  hello();
}
```

### Module Declaration

A **module unit** is a translation unit that have a **module declaration**. In a module unit, no statements or preprocessing directives can appear before the module declaration. A module declaration provides a **module name**.

```c++
module NAME;
```

## How to build projects using modules [Clang]

### Simple Hello World

```c++
// Hello.cpp
module;
#include <iostream>
export module Hello;
export void hello() {
  std::cout << "Hello World!\n";
}

// use.cpp
import Hello;
int main() {
  hello();
}
```

Then, on the command line, invoke `clang++` like:

```bash
~ % clang++ -std=c++20 Hello.cppm --precompile -o Hello.pcm
~ % clang++ -std=c++20 use.cpp -fmodule-file=Hello=Hello.pcm Hello.pcm -o Hello.out
~ % ./Hello.out
Hello World!
```

### Module and module unit

A module consists of one or more module units. A module unit is a special kind of translation unit. A module unit should almost always start with a module declaration. The syntax of the module declaration is:

```
[export] module module_name[:partition_name];
```


