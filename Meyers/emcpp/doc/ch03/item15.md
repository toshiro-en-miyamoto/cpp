# Item 15: Use `constexpr` whenever possible

## `constexpr` objects

`constepxr` objects are `const`, and they have values that are knwon at compile time.

-   they may be placed in read-only memory
-   they can be used as *integral constant expressions*, such as
    -   array sizes
    -   integral template arguments
    -   enumerator values
    -   alignment specifiers

The [`constexpr` specifier](https://en.cppreference.com/w/cpp/language/constexpr)
impiles:

-   `const` when used in an object declaration
-   `inline` when used in a static data member declaration
