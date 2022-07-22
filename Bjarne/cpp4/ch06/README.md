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

### 6.3.5. Initialization

An initializer can use one of four syntactic styles:

```c++
X a1 {v}
X a2 = {v};
X a3 = v;
X a4(v);
```

Of these, only the first can be used in every context, and I strongly recommend its use.

There is no advantage to using `{}` initialization, and one trap, when using `auto` to get the type determined by the initializer. The trap is that if the initializer is a `{}`-list, we may not want its type deduced.

```c++
auto z1 {99};   // z1 is an initializer-list<int>
auto z2 = 99;   // z2 is an int
```

So prefer `=` when using `auto`.

It is possible to define a class so that an object can be initialized by a list of values and alternatively be constructed given a couple of arguments that are not simply values to be stored.

```c++
std::vector<int> v1 {99};   // a vector of 1 element wit the value 99
atd::vector<int> v2(99);    // a vector of 99 elements each with default 0
std::vector<std::string> v3 {"xyz"};    // a vector of 1 element
std::vector<std::string> v4("xyz");     // error; no ctor taking a string
```

So, prefer `{}` initialization over alternatives unless you have a strong reason not to.

The empty initializer list, `{}`, is used to indicate that a default value is desired.

```c++
int i {};               // zero for integral types
char* p {};             // nullptr
std::vector<int> v {};  // an empty vector
std::string str {};     // "", a string with length zero
```

### 6.3.5.2. Initializer Lists

More complicated objects can require more than one value as an initializer.

```c++
int a[] {1, 2};                         // use array initializer
std::complex<double> z {0, pi};         // use constructor
std::vector<double> v {0.0, 1.1};       // use list constructor

struct S
{
    int x;
    std::string s;
};

S s { 1, "Helios" };                    // use struct initializer
```

In a declaration, an empty pair of parentheses, `()`, always means *function*.

```c++
std::complex<double> z1 {1, 2};     // use constructor to {1, 2}
std::complex<double> z2 (1, 2);     // == z1
std::complex<double> z3 {};         // use constructor to the default
std::complex<double> z4 ();         // function declaration
```

When using `auto`, a `{}`-list has its type deduced to `std::initializer_list<T>`.

```c++
auto x1 {1, 2, 3, 4};       // x1 is an initializer_list<int>
auto x2 {1.1, 2.2, 3.3};    // x2 is an initializer_list<double>
auto x3 {1.0, 2};           // error: cannot deduce the type
```

## 6.4. Objects and Values

We need a name for *something in memory*. This is the simplest and most fundamental notion of an *object*. That is, an object is a contiguous region of storage; an *lvalue* is an expression that refers to an object. [...] Not every *lvalue* may be used on the left-hand side of an assignment; an *lvalue* can refer to a constant.

### 6.4.1. Lvalues and Rvalues

To complement the notion of an *lvalue*, we have the notion of an *rvalue*. Roughly, *rvalue* means "a value that is not an *lvalue*," such as a temporary value (e.g., the value returned by a function).

There are two properties that matter for an object when it comes to addressing, copying, and moving:

- *Has identity*: The program has the name of, pointer to, or reference to the object so that it is possible to determine if two objects are the same, whether the value of the object has changed, etc.
- *Is Movable*: The object may be moved from.
<style>
    td { text-align: center; }
</style>
<table>
    <tr>
        <td></td>
        <td>has identity</td>
        <td>no identity</td>
    </tr>
    <tr>
        <td>is movable</td>
        <td colspan="2">rvalue</td>
    </tr>
    <tr>
        <td>not movable</td>
        <td>lvalue</td>
        <td>(n/a)</td>
    </tr>
</table>

For practical programming, thinking in terms of *rvalue* and *lvalue* is usually sufficient. Note that every expression is either an *lvalue* or an *rvalue*, but not both.

<table>
    <tr>
        <td></td>
        <td>has identity</td>
        <td>no identity</td>
    </tr>
    <tr>
        <td>is movable</td>
        <td>xvalue</td>
        <td>pure rvalue</td>
    </tr>
</table>

```c++
void f(std::vector<std::string>& vs)
{
    std::vector<string> v2 = std::move(vs);
}
```

Here, `std::move(vs)` is an *xvalue*: it clearly has identity (we can refer to it as `vs`), but we have explicitly given permission for it to be moved from by calling `std::move()`.

### 6.4.2. Lifetimes of Objects

- *Automatic* objects: are allocated on the stack; each call of the function gets its own *stack frame* to hold its automatic objects. An automatic object declared in a function is created when its definition is encountered and destroyed when its name goes out of scope.
- *Static* objects: are created and initialized once (only) and live until the program terminates. A static object has the same address throughout the life of a program execution.
- *Free store*: Using the `new` and `delete` operators, we can create objects whose lifetimes are controlled directly.
- *Temporary* objects: such as intermediate results in a computation or an object used to hold a value for a reference to `const` argument.





