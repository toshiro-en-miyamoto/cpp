# References

If we didn’t want to copy the values from `v` into the variable `x`, but rather just have `x` refer to an element, we could write:

```c++
int v[] = {1, 2, 3, 4};
for (auto& x : v) ++x;
// now {2, 3, 4, 5}
```

In a declaration `auto& x`, the unary suffix `&` means "reference to." A reference is similar to a pointer, except that you don’t need to use a prefix `*` to access the value referred to by the reference.

References are particularly useful for specifying function arguments.

```c++
double sum(const std::vector<double>&);
```

Functions taking `const` references are very common.

[1.7 Pointers, Arrays, and References: Tour3, Stroustrup]

When a function's return type is lvalue reference, the function call expression becomes an lvalue expression:

```c++
// constexpr char& std::string::operator[]( size_type pos );

auto char_at = []
(std::string& s, std::string::size_type pos) -> char&
{
    return s[pos];
};

std::string s = "Test";
char_at(s, 0) = 'R';
expect(s == "Rest");
```

[Lvalue references: cppreference](https://en.cppreference.com/w/cpp/language/reference)


## [`std::reference_wrapper`](https://en.cppreference.com/w/cpp/utility/functional/reference_wrapper)

`std::reference_wrapper` is a class template that wraps a reference in a copyable, assignable object. It is frequently used as a mechanism to store references inside standard containers (like `std::vector`) which cannot normally hold references.

```c++
std::list<int> l(9);
std::iota(l.begin(), l.end(), -4);
std::vector<std::reference_wrapper<int>> v {l.begin(), l.end()};
```

Specifically, `std::reference_wrapper` is a *CopyConstructible* and *CopyAssignable* wrapper around a reference to object or reference to function of type `T`. Instances of `std::reference_wrapper` are objects (they can be copied or stored in containers) but they are implicitly convertible to `T&`, so that they can be used as arguments with the functions that take the underlying type by reference.

```c++
std::cout << "vector: ";
for (auto const& i : v) {   // implicitly convertible to `T&`
    std::cout << i << " ";
}
std::cout << std::endl;
```

## `std::thread` arguments

With a function taking a reference,

```c++
void plus(std::vector<int>& v, int d)
{
    std::ranges::for_each(v, [=](int& n) { n += d; });
}
```

you might want call it in a thread.

```c++
std::vector<int> v {0, 1, 2, 3};
constexpr int delta = 1;
// std::thread t1 {plus, v, delta}; >> compile error
// expecting {1, 2, 3, 4}
```

[`std::thread` in cppreference.com](https://en.cppreference.com/w/cpp/thread/thread/thread) says:

- The arguments to the thread function are moved or copied by value.
- If a reference argument needs to be passed to the thread function, it has to be wrapped (e.g., with `std::ref` or `std::cref`).

```c++
std::vector<int> v {0, 1, 2, 3};
constexpr int delta = 1;
std::thread t1 {plus, std::ref(v), delta};
// expecting {1, 2, 3, 4}
```

Function templates [`std::ref` and `std::cref`](https://en.cppreference.com/w/cpp/utility/functional/ref), `<functional>`, are helper functions that generate an object of type `std::reference_wrapper`, using template argument deduction to determine the template argument of the result.

```c++
template< class T >
constexpr std::reference_wrapper<T> ref( T& t ) noexcept;
```
