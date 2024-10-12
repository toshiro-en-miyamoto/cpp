# C++ Programming Language

- [Bjarne/cpp4] *The C++ Programming Language, 4th Edition* by Bjarne Stroustrup (2013), presents concise, *pure C++11* examples.
- [Bjarne/ppp3] *Programming: Principles and Practice Using C++, 3rd Edition* by Bjarne Stroustrup (2024), for someone who programs in another language and wants to learn C++.
- [C++ Best Practices](https://github.com/cpp-best-practices) provide additional documentation and supporting tools that reflect the guidelines in Jason Turner's C++ Best Practices book.
- [Lakos/embrace] *Embracing Modern C++ Safely* by John Lakos, et al. (2021)
- [Meyers/empower] 

- *C++ Software Design Patterns (Early Release)* by Klaus Iglberger (August 2021), shows and discusses implementation details [using modern C++ features], but (hopefully) does not get lost in them and always remains focused on the big picture, on software design and design patterns.
- *Elements of Programming* by Paul McJones & Alexander Stepanov (June 2009, ISBN-13: 978-0-321-63537-2), applies the deductive method to programming by affiliating programs with the abstract mathematical theories that enable them to work.
- *Functional Programming in C++* by Ivan Cukic (January 2019, ISBN 9781617293818), is meant to teach you an alternative way of using a language (C++): a way that’s different enough that it’ll often feel like you’re using a new language.

## GCC / Clang

Reference: [C++ Best Practices]

Use these warning options:

```
-Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic
```

- `-pedantic` warn on language extensions
- `-Wall` `-Wextra` reasonable and standard
- `-Wshadow` warn the user if a variable declaration shadows one from a parent context
- `-Wnon-virtual-dtor` warn the user if a class with virtual functions has a non-virtual destructor. This helps catch hard to track down memory errors.

and consider the following:

| `-W` option         | G | C | reasons of the warning  |
|---------------------|:-:|:-:|-------------------------|
| old-style-cast      | * | * | c-style casts |
| cast-align          | * | * | potential performance problem casts |
| unused              | * | * | anything being unused |
| overloaded-virtual  | * | * | you overload a virtual function |
| pedantic            | * | * | non-standard C++ is used |
| conversion          | * | * | type conversions that may lose data |
| sign-conversion     | * | * | sign conversions |
| duplicated-cond     | * |   | if/else chain has duplicated conditions |
| duplicated-branches | * |   | if/else branches have duplicated code |
| logical-op          | * |   | logical operations being used where bitwise were probably wanted |
| null-dereference    | * |   | a null dereference is detected |
| useless-cast        | * |   | you perform a cast to the same type |
| double-promotion    | * | * | float is implicitly promoted to double |
| format=2            | * | * | security issues around functions that format output (i.e., printf) |
