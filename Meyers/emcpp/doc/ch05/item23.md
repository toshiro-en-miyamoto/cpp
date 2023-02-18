# Item 23: Understand s`td::move` and `std::forward`.

`std::move` and `std::forward` generate no executable code. Not a single byte.

- `std::move` unconditionally casts its argument to an rvalue
- `std::forward` performs this cast only if a particular condition is fulfilled

```c++
// <utility>

template< class T >
constexpr std::remove_reference_t<T>&& move( T&& t ) noexcept;

template< class T >
constexpr T&& forward( std::remove_reference_t<T>& t ) noexcept;

template< class T >
constexpr T&& forward( std::remove_reference_t<T>&& t ) noexcept;
```

If the type `T` is a reference type, `std::remove_reference_t`  is the type referred to by `T`. Otherwise type is `T`.

| type argument                             | type
|-------------------------------------------|-------------
| `std::remove_reference_t<int>`            | `int`
| `std::remove_reference_t<int&>`           | `int`
| `std::remove_reference_t<int&&>`          | `int`
| `std::remove_reference_t<const int>`      | `const int`
| `std::remove_reference_t<const int&>`     | `const int`
| `std::remove_reference_t<const int&&>`    | `const int`

This code compiles. This code links. This code runs. This code sets the data member `value` to the content of `text`.

```c++
class Annotation {
public:
    explicit Annotation(const std::string text)
    : value(std::move(text))
    {}
private:
    std::string value;
};
```

But `text` is not moved into `value`, it's *copied*. The member initialization `value(std::move(text))` invokes the copy constructor in `std::string`. Even though the result of `std::move(text)` is `const std::string&&`,

- the rvalue can’t be passed to `std::string`’s move constructor, because the move constructor takes an rvalue reference to a non-const `std::string`, but
- the rvalue can be passed to the copy constructor, because an lvalue-reference-to-const is permitted to bind to a const rvalue.

