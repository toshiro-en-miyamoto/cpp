# Chapter 6. Types and Declarations

## 6.2. Types

*Built-in types*:
- *fundamental types*:
    - a boolean type: `bool`
    - character types: `char`, `wchar_t`, `char16_t`
    - integer types: such as `int`, `long`
    - floating-point types: such as `double`
    - an incomplete-type: `void`
- *pointers*: such as `int*`
- *arrays*: such as `char[]`
- *references*: such as `double&`, `vector<int>&&`

*User-defined types*:
- data structures and classes: `struct`, `class`
- enumeration types: `enum`, `enum class`

## 6.3. Declarations

| Kind       | Declaration                | must be defined elsewhere
|------------|----------------------------|--------------------------------
| function   | `double sqrt(double);`     | `double sqrt(double d) { ...}`
| variable   | `extern int error_number;` | `int error_number = 1;`
| type name  | `struct User;`             | `struct User { ... };`


