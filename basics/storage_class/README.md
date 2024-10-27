# Storage Classes

The following keywords are [storage class specifiers](https://en.cppreference.com/w/cpp/language/storage_duration):

- `static`
- `thread_local`
- `extern`

The storage class specifiers are a part of the *decl-specifier-seq* of a name's declaration syntax. Together with the scope of the name, they control two independent properties of the name: its *storage duration* and its *linkage*.

> `mutable` is a storage class specifier, but it has no effect on storage duration.

## Declaration specifiers

[decl-specifier-seq](https://en.cppreference.com/w/cpp/language/declarations) is a sequence of the following whitespace-separated specifiers, in any order, including

Various specifiers:

- `inline`, `virtual`, `explicit` in function declarations
- `inline` in variable declarations
- `friend` in class and function declarations
- `constexpr`
  - in variable definitions
  - in declaration of static data member of literal type
  - in function and function template declarations
- `consteval`
  - in function and function template declarations
- `constinit`
  - in declaration of a variable with static storage duration
  - in declaration of a variable with thread storage duration

Storage class specifiers

- `static`
- `thread_local`
- `extern`
- `mutable`

Type specifiers:

- `class` specifier
- `enum` specifier
- simple type specifiers:
  - `void`
  - integral types: `char`, `int`, `double`, `unsigned` etc.
  - `auto`
- cv qualifier

## Storage duration

The storage duration is the property of an object that defines the minimum potential lifetime of the storage containing the object. The storage duration is determined by the construct used to create the object and is one of the following:

- static storage duration 
- thread storage duration 
- automatic storage duration
- dynamic storage duration

| object                              | static  | thread  | automatic | dynamic |
|-------------------------------------|:-------:|:-------:|:---------:|:-------:|
| objects introduced by declarations  | yes     | yes     | yes       |         | 
| temporary objects                   | yes     | yes     | yes       |         |
| objects created by a new expression |         |         |           | yes     |
| implicitly created objects          |         |         |           | yes     |

The storage duration categories apply to references as well.


All variables in a program have one of the following [storage durations](https://en.cppreference.com/book/storage_durations):

- *static storage duration*: The variable is allocated when the program begins and deallocated when the program ends. Only one instance of the variable exists. All global variables have this storage duration, plus those declared with `static` or `extern`.
- *thread storage duration*: The variable is allocated when the thread begins and deallocated when the thread ends. Each thread has its own instance of the variable. Only variables declared `thread_local` have this storage duration. `thread_local` can appear together with `static` or `extern` to adjust linkage.
- *automatic storage duration*: The variable is allocated at the beginning of the enclosing code block and deallocated on end. All non-global variables have this storage duration, except those declared `static`, `extern` or `thread_local`.
- *dynamic storage duration*: The variable is allocated and deallocated per request by using dynamic memory allocation functions.

## Linkage

A name can have *external linkage*, *module linkage*, *internal linkage*, or *no linkage*.

- An entity whose name has external linkage can be re-declared in another translation unit, and the re-declaration can be attached to a different module.
- An entity whose name has module linkage can be re-declared in another translation unit, as long as the re-declaration is attached to the same module.
- An entity whose name has internal linkage can be re-declared in another scope in the same translation unit.
- An entity whose name has no linkage can only be re-declared in the same scope.

### Static storage duration

The [`static` keyword](https://learn.microsoft.com/en-us/cpp/cpp/storage-classes-cpp?view=msvc-170) can be used to declare:

- variables
  - at global or namespace scope
  - at class scope
  - at local scope
- functions
  - at global or namespace scope
  - at class scope

Static duration means that the object or variable is allocated when the program starts and is deallocated when the program ends.

- External linkage means that the name of the variable is visible from outside the file where the variable is declared.
- Conversely, internal linkage means that the name isn't visible outside the file where the variable is declared.

By default, an object or variable that is defined in the global namespace has static duration and external linkage. The static keyword can be used in the following situations.

- When you declare a variable or function at file scope (global and/or namespace scope), the `static` keyword specifies that the variable or function has internal linkage.
- When you declare a variable at file scope (global and/or namespace scope), the variable has static duration and the compiler initializes it to a default value unless you specify another value.

  ```c++
  #define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
  #include "../doctest/doctest.h"

  // a variable at global scope
  int global_counter = 0;

  TEST_CASE("global counting") {
    CHECK(++global_counter == 1);
    CHECK(++global_counter == 2);
    CHECK(++global_counter == 3);
  }
  ```

- When you declare a variable in a function, the `static` keyword specifies that the variable retains its state between calls to that function.

  ```c++
  // a function at global scope
  int local_counting() {
    // a static variable in a function
    static int counter = 0;
    return ++counter;
  }

  TEST_CASE("local counting") {
    CHECK(local_counting() == 1);
    CHECK(local_counting() == 2);
    CHECK(local_counting() == 3);
  }
  ```

- When you declare a data member in a class declaration, the `static` keyword specifies that one copy of the member is shared by all instances of the class. A static data member must be defined at file scope. An integral data member that you declare as `const static` can have an initializer.

  ```c++
  // a struct at global scope
  struct Counter {
    // a static data member, one copy of the
    // member is shared by all instances
    static int value;
  };

  // A static member must be defined at file scope.
  int Counter::value = 0;

  TEST_CASE("counter struct") {
    Counter c1;
    Counter c2;
    c1.value++;
    c2.value++;
    CHECK(c1.value == c2.value);
    CHECK(c1.value == 2);
  }
  ```

- When you declare a member function in a class declaration, the `static` keyword specifies that the function is shared by all instances of the class.
  - A `static` member function can't access an instance member because the function doesn't have an implicit `this` pointer.
  - To access an instance member, declare the function with a parameter that's an instance pointer or reference.
