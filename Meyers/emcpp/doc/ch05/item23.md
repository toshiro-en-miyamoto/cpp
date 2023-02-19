# Item 23: Understand s`td::move` and `std::forward`.

`std::move` and `std::forward` generate no executable code. Not a single byte.

- `std::move` unconditionally casts its argument to an rvalue
- `std::forward` performs this cast only if a particular condition is fulfilled

## `std::move`

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

## `std::forward`

`std::forward` is a conditional cast. The most common scenario is a function template taking a universal reference parameter that is to be passed to another function:

```c++
void process(const Widget& lvalArg);
void process(Widget&& rvalArg);

template<typename T>
void logAndProcess(T&& param);

void invoke()
{
    Widget w;
    logAndProcess(w);               // call with lvalue
    logAndProcess(std::move(w));    // call with rvalue
}
```

Suppose that `logAndProcess` calls `process`:

```c++
template<typename T>
void logAndProcess(T&& param)
{
    process(param);
}
```

`param` that `logAndProcess` takes therefore is an lvalue. Every call to `process` inside `logAndProcess` will thus want to invoke the lvalue overload for process.

> Recall a parameter is always an lvalue, even if its type is an rvalue reference. That is, given `void f(Widget&& w)`, the parameter `w` is an lvalue, even though its type is rvalue-reference-to-`Widget`.

```c++
void f(Widget&& w);     // w is an lvalue
```

When we call `logAndProcess` with an lvalue, we naturally expect that lvalue to be forwarded to `process` as an lvalue, and when we call `logAndProcess` with an rvalue, we expect the rvalue overload of `process` to be invoked.

We need a mechanism for `param` to be cast to an rvalue if and only if the argument with which `param` was initialized — the argument passed to `logAndProcess` — was an rvalue. This is precisely what `std::forward` does.

```c++
template<typename T>
void logAndProcess(T&& param)
{
    process(std::forward<T>(param));
}
```

That’s why `std::forward` is a conditional cast: it casts to an rvalue only if its argument was initialized with an rvalue.

The use of `std::move` conveys an unconditional cast to an rvalue, while the use of `std::forward` indicates a cast to an rvalue only for references to which rvalues have been bound. Those are two very different actions.
