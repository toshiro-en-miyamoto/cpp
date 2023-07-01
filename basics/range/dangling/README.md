# [`std::ranges::dangling`](https://en.cppreference.com/w/cpp/ranges/dangling)

With the following code,

```c++
#include <array>
#include <algorithm>
#include <type_traits>
#include <ranges>

int main()
{
  auto array_by_value = [] { return std::array{0, 1, 2, 3}; };
  static_assert(array_by_value()[3] == 3);
  auto it_by_value = std::ranges::max_element(array_by_value());
  static_assert(std::is_same_v<
    decltype(it_by_value), std::ranges::dangling
  >);
  *it_by_value == 3;
}
```

Dereferencing the return iterator (`*it_by_value`) doesn't compile:

```
compile error: no operator "*" matches the operand
  the operand type: std::ranges::dangling
```

`std::ranges::dangling` is a placeholder type and empty class type - it does't define the dereferencing operator `*`.

When some constrained algorithms that usually return an iterator or a subrange of a `range` take a particular rvalue range argument that does not model `std::ranges::borrowed_range`, **`std::ranges::dangling` will be returned instead** to avoid returning potentially dangling results.

`std::ranges::max_element()`, a constrained algorithm used in the preceding code, usually returns an iterator of type `std::ranges::borrowed_iterator_t<R>`:

```c++
constexpr ranges::borrowed_iterator_t<R>
  max_element( R&& r, Comp comp = {}, Proj proj = {} );
```

The algorithm taking an lvalue returns a `ranges::borrowed_iterator_t`:

```c++
using the_array_type = const std::array<int, 4>;
constexpr the_array_type static_array{0, 1, 2, 3};

int main()
{
  constexpr auto it_persist = std::ranges::max_element(static_array);
  static_assert(std::is_same_v<
    decltype(it_persist),
    std::ranges::borrowed_iterator_t<the_array_type &> const
  >);
  static_assert(*it_persist == 3);
}
```

The first code in this section, however, has passed a temporary object to `std::ranges::max_element()`:

```c++
  auto array_by_value = [] {
    return std::array{0, 1, 2, 3};
    // (1) a temporary object for the prvalue array is created
    // and returned to the caller,
  };

  auto it_by_value = std::ranges::max_element(
    array_by_value()    // the temporary object is passed
  );
  // (2) then std::array's destructor is called for the temporary object

  // as the temporary object had gone at this point,
  // the resulting iterator should be a std::ranges::dangling
  static_assert(std::is_same_v<
    decltype(it_by_value), std::ranges::dangling
  >);
```

1. [Returning by value](https://en.cppreference.com/w/cpp/language/return) may involve construction and copy/move of a temporary object.
2. All temporary objects are destroyed as the last step in evaluating the full-expression that (lexically) contains the point where they were created. [[Temporary object lifetime](https://en.cppreference.com/w/cpp/language/lifetime)]
