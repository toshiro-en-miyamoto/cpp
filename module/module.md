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

A module declaration provides a **module name**. The module name consists of one or more identifiers separated by dots (for example: `datamodel`, `datamodel.person`, `datamodel.address`...). Dots have no intrinsic meaning, however they are used informally to represent hierarchy.

A **named module** is the collection of module units wit the same module name.

- Module units whose declaration has the keyword `export` are termed **module interface units**;
- all other module units are termed **module implementation units**.

For every named module, there must be exactly one module interface unit that specifies no module partition; this module unit is termed the **primary module interface unit**.

```c++
// interface.cpp
export module A;      // declares the primary module interface unit for name module 'A'

// impl1.cpp
module A;             // declares a module implementation unit for named module 'Hello'

// impl2.cpp
module A;             // declares another module implementation unit for named module 'Hello'
```

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

// DataModelAddress.cppm
export module datamodel.address;
export namespace DataModel {
  class Address { /* ... */ };
}

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

A benefit of structuring your code with submodules is that clients can import either everything at once or only specific parts they want to use. For example, if client code needs access to everything in the datamodel module, then the following import declaration is the easiest:

```c++
import datamodel;
```

On the other hand, if client code is only interested in using the Address class, then the following import declaration suffices:

```c++
import datamodel.address;
```

### Module Partitions

A module can have **module partition units**.

- A module partition represents exactly one module unit;
- two module units cannot designate the same module partition.

They are visible only from inside the named module;

- Translation units outside the named module cannot import a module partition directly.
- A module partition can be imported by module units of the same named module.

```c++
// A-B.cpp
export module A:B;    // declares the partition module interface unit

export char const* Hello()
{
  return "Hello";
}
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


