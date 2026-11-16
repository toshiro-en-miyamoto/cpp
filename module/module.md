# C++20 Modules

- [C++Ref] [Modules (since C++20), cppreference](https://www.cppreference.com/cpp/language/modules)
- [Clang] [Standard C++ Module, Clang](https://clang.llvm.org/docs/StandardCPlusPlusModules.html#how-to-build-projects-using-modules)
- [Gregoire2024] Professional C++, 6th Edition; By Marc Gregoire; John Wiley & Sons, 2024; ISBN: 9781394193172 (Paperback)

## Modules

Here is an example of a module interface file called `Person.cppm`, defining a `person` module and exporting a `Person` class [Gregoire2024]:

```c++
// Person.cppm: a primary module interface unit
export module person;      // module declaration
import std;                // import declaration
export class Person        // export declaration
{
  public:
    Person(std::string firstName, std::string lastName);
    const std::string& getFirstName() const;
    const std::string& getLastName() const;
  private:
    std::string m_firstName;
    std::string m_lastName;
}
```
```c++
// Person.cpp: a module implementation unit
module person;             // module declaration without export

Person::Person(std::string firstName, std::string lastName)
  : m_firstName{std::move(firstName)}, m_lastName{std::move(lastName)}
  {}

const std::string& Person::getFirstName() const {
  return m_firstName;
}
const std::string& Person::getLastName() const {
  return m_lastName;
}
```
```c++
// main.cpp
import person;             // import declaration
import std;

int main()
{
  Person webb{"Jason", "Webb"};
  std::println("{}, {}", webb.getLastName(), webb.getFirstName());
}
```

### Module Declaration [C++Ref]

A **module unit** is a translation unit that have a **module declaration**. In a module unit, no statements or preprocessing directives (other than `#line`) can appear before the module declaration.

A module declaration provides a **module name**.

- The module name and partition name follow the rules for a C++ identifier, except that they may contain one or more period (`.`) characters.
- Note that a `.` in the name has no semantic meaning and does not imply any hierarchy.

A **named module** is the collection of module units wit the same module name.

- Module units whose declaration has the keyword `export` are termed **module interface units**;
- all other module units are termed **module implementation units**.

For every named module, there must be exactly one module interface unit that specifies no module partition; this module unit is termed the **primary module interface unit**.

```c++
// interface.cpp
export module A;      // declares the primary module interface unit for the module 'A'

// impl1.cpp
module A;             // declares a module implementation unit for the module 'A'

// impl2.cpp
module A;             // declares another module implementation unit for the module 'A'
```

Here is a summary table of the syntax and term:

| Module | Syntax | Term of Module Declaration |
| ---- | ---- | ---- |
| `export` | `module A;` | the primary module interface unit for the module `A` |
| | `module A;` | a module implementation unit for the module `A` |
| `export` | `module A:B;` | the module partition interface unit for the module `A` partition `B` |
| | `module A:B;` | the module partition implementation unit for the module `A` partition `B` |
| | `module;` | used when the module unit has a global module fragment |

> Note: A module can have multiple partition units (explained later in this article). A module partition can be imported by module units of the same named module. Translation units outside the named module cannot import a module partition directly.

### Submodules [Gregoire2024]

If a namespace is explicitly exported with the `export` keyword, everything inside that namespace is automatically exported as well. For example, the following code snippet export the entire `DataModel` namespace; hence, there is no need to explicitly export the individual classes and type alias:

```c++
// DataModel.cppm
export module datamodel;
import std;
export namespace DataModel
{
  class Person { /* ... */ };
  class Address { /* ... */ };
  using Persons = std::vector<Person>;
}
```

This namespace can be restructured by defining two submodules: `datamodel.person` and `datamodel.address`. As [C++Ref] describes, dots in a module name have no intrinsic meaning, however that makes it possible to structure your modules in any hierarchy you want.

```c++
// DataModelPerson.cppm
export module datamodel.person;
export namespace DataModel {
  class Person { /* ... */ };
}
```
```c++
// DataModelAddress.cppm
export module datamodel.address;
export namespace DataModel {
  class Address { /* ... */ };
}
```
```c++
// DataModel.cppm
export module datamodel;
export import datamodel.person;
export import datamodel.address;
import std;
export namespace DataModel
{
  using Persons = std::vector<Person>;
}
```

`export import ...` imports a module and immediately exports the module.

A benefit of structuring your code with submodules is that clients can import either everything at once or only specific parts they want to use. For example, if client code needs access to everything in the `datamodel` module, then the following import declaration is the easiest:

```c++
import datamodel;
```

On the other hand, if client code is only interested in using the `Address` class, then the following import declaration suffices:

```c++
import datamodel.address;
```

### Module Partitions [C++Ref]

A module can have **module partition units**.

- A module partition represents exactly one module unit;
- two module units cannot designate the same module partition.

They are visible only from inside the named module;

- Translation units outside the named module cannot import a module partition directly.
- A module partition can be imported by module units of the same named module.

```c++
// A-B.cppm
export module A:B;      // (*1) the module interface unit for 'A:B'
```
```c++
// A-C.cpp
module A:C;             // (*2) the module implementation unit for 'A:C'
```
```c++
// A.cppm
export module A;        // (*3) the primary module interface unit for 'A'
export import :B;       // (*4)
import :C;              // (*5)
// export import :C;    // ERROR: Cannot export a module implementation unit.
```

- (*1): Module partitions can be module interface units when their module declarations have `export`. Module units whose declaration has the keyword `export` are termed *module interface units*.
- (*2): Module units whose declaration does not have the keyword `export` are termed *module implementation units*.
- (*3): For every named module, there must be exactly one module interface unit that specifies no module partition; this module unit is termed the *primary module interface unit*.
- (*4): Module partition interface units can be `export-import`ed by only the primary module interface unit, and their exported statements will be visible from outside the named module by importing the module.
- (*4) and (*5): All definitions and declarations in a module partition are visible by the importing module unit, whether exported or not.

## How to build projects using modules [Clang]

### Simple Hello World

```c++
// Hello.cppm
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

### File name requirements

Currently, Clang requires the file name of an importable module unit to have `.cppm` (or `.ccm`, `.cxxm`, `.c++m`) as the file extension. However, the behavior is inconsistent with other compilers.

An importable module unit refers to either

- a module interface unit, or
- a module unit whose module declaration is `module module_name:partition_name`.

```c++
// M.cppm
export module M;            // the primary module interface for 'M'
export import :part;
import :impl_part;
export void Hello();

// Impl.cpp
module;
#include <iostream>
module M;
void Hello() {
  std::cout << "Hello ";
}

// interface_part.cppm
export module M:part;       // the interface for 'M:part'
export void World();

// impl_part.cppm
module;
#include <iostream>
#include <string>
module M:impl_part;         // the implementation for 'M:part'
import :part;

std::string W = "World.";
void World() {
  std::cout << W << std::endl;
}

// User.cpp
import M;
int main() {
  Hello();
  World();
  return 0;
}
```

### Reachability of internal partition units

The internal partition units are sometimes called *implementation partition units* in other documentation. However, the name may be confusing since implementation partition units are not implementation units.

