# `auto`

*Invisible* proxy types can cause `auto` to deduce the wrong type for an initializing expression.

```c++
#include <vector>
#include <type_traits>
#include <boost/ut.hpp>

int main()
{
    using namespace boost::ut;
    using namespace boost::ut::bdd;

    "operator[] of vector<bool>"_test = [] {
        given("four-bit vector<bool>") = [] {
            std::vector<bool> v4 {
                true, false, true, false
            };
            when("initializing a variable as bool") = [&] {
                bool b0 = v4[0];
                expect(b0 == true);
                expect(std::is_same_v<decltype(b0), bool>);
            };
            when("initializing a variable as auto") = [&] {
                auto b0 = v4[0];
                expect(b0 == true);
                expect(std::is_same_v<
                    decltype(b0),
                    std::vector<bool>::reference
                >);
            };
        };
    };
}
```

In the code using `auto`, the type of `b0` is no longer `bool`. Though `std::vector<bool>` conceptually holds bools, `operator[]` for `std::vector<bool>` doesn’t return a reference to an element of the container (which is what `std::vector::operator[]` returns for every type except `bool`). Instead, it returns an object of type `std::vector<bool>::reference` (a class nested inside `std::vector<bool>`).

`std::vector<bool>::reference` is an example of a *proxy class*: a class that exists for the purpose of emulating and augmenting the behavior of some other type.

Some proxy classes are designed to be apparent to clients. That’s the case for `std::shared_ptr` and `std::unique_ptr`, for example. Other proxy classes are designed to act more or less *invisibly*. `std::vector<bool>::reference` is an example of such invisible proxies.

As a general rule, invisible proxy classes don’t play well with `auto`. Objects of such classes are often not designed to live longer than a single statement, so creating variables of those types tends to violate fundamental library design assumptions. That’s the case with `std::vector<bool>::reference`.

But how can you recognize when proxy objects are in use?

## `std::vector<bool>`

Look into a detail of [`std::vector<bool>`](https://en.cppreference.com/w/cpp/container/vector_bool) at `cppreference.com`.

```c++
template<class Allocator>
class vector<bool, Allocator>;
```

You can find descriptions regarding the proxy class:

| Member type       | Definition
|-------------------|-------------
| `value_type`      | `bool`
| `size_type`       | implementation-defined
| `reference`       | *proxy class* representing a reference to a single bool
| `const_reference` | `bool`

`std::vector<bool>` exposes class [`std::vector<bool>::reference`](https://en.cppreference.com/w/cpp/container/vector_bool/reference) as a method of accessing individual bits.

```c++
std::vector<bool>::reference;
```

| Member            | functions
|-------------------|-----------
| `operator=`       | assigns a `bool` to the referenced bit
| `operator bool`   | returns the referenced bit
| `flip`            | flips the referenced bit

For example:

```c++
int main()
{
    "operator[] of vector<bool>"_test = [] {
        given("four-bit vector<bool>") = [] {
            std::vector<bool> v4 {
                true, false, true, false
            };
            when("inverting a bit") = [&] {
                std::vector<bool>::reference b0 = v4[0];
                expect(b0 == true);
                b0.flip();
                expect(b0 == false);
            };
        };
    };
}
```

## `std::bitset`

The class template [`bitset`](https://en.cppreference.com/w/cpp/utility/bitset) represents a fixed-size sequence of `N` bits.

```c++
template< std::size_t N >
class bitset;
```

| Member        | types
|---------------|-------
| `reference`   | proxy class representing a reference to a bit

```c++
std::bitset<N>::reference
```

| Member            | functions
|-------------------|------------
| `operator=`       | assigns a `bool` to the referenced bit
| `operator bool`   | returns the referenced bit
| `operator~`       | returns inverted referenced bit
| `flip`            | flips the referenced bit
