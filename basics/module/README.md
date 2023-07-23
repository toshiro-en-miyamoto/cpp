# Standard C++ Modules

A [module](https://releases.llvm.org/16.0.0/tools/clang/docs/StandardCPlusPlusModules.html) consists of one or more *module units*. A module unit is a special translation unit. Every module unit must have a *module declaration*, which declares that the current translation unit is a module unit. The syntax of the module declaration is:

```c++
// the syntax:
//    [export] module module_name[:partition_name];

// Primary module interface unit
export module module_name;

// Module implementation unit
module module_name;

// Module interface partition unit
export module module_name:partition_name;

// Internal module partition unit
module module_name:partition_name;
```

The syntax of `module_name` and `partition_name` in regex form corresponds to `[a-zA-Z_][a-zA-Z_0-9\.]*`. In particular, a literal dot `.` in the name has no semantic meaning (e.g. implying a hierarchy).

- A module should have one and only one primary module interface unit.
- A module could have multiple module implementation units with the same declaration.
- The `partition_name` should be unique within any given module.

In this document, we use the following umbrella terms:

| | Primary module interface unit | Module implementation unit | Module interface partition unit | Internal module partition unit
|:---:|:---:|:---:|:---:|:---:|
| module interface unit   | ✔ |   | ✔ |  
| importable module unit  | ✔ |   | ✔ | ✔
| module partition unit   |   |   | ✔ | ✔
