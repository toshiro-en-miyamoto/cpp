# Dynamic Memory Management

## Allocators

[Allocators](https://en.cppreference.com/w/cpp/memory) are class templates encapsulating memory allocation strategy. This allows generic containers to decouple memory management from the data itself.

### `std::allocator`

The `std::allocator` class template is the default *Allocator* used by all standard library containers if no user-specified allocator is provided. The default allocator is stateless, that is, all instances of the given allocator are interchangeable, compare equal and can deallocate memory allocated by any other instance of the same allocator type.

```c++
template<typename T>
struct allocator
{
    [[nodiscard]] constexpr T* allocate(std::size_t n);
    constexpr void deallocate(T* p, std::size_t n);
};
```

#### 'deallocate()`

Deallocates the storage referenced by the pointer `p`, which must be a pointer obtained by an earlier call to `allocate()`.

The argument `n` must be equal to the first argument of the call to `allocate()` that originally produced `p`; otherwise, the behavior is undefined.

#### `allocate()`

Allocates `n * sizeof(T)` bytes of uninitialized storage by calling `::operator new(std::size_t)` or `::operator new(std::size_t, std::align_val_t)` (since C++17), but it is unspecified when and how this function is called.

Then, this function creates an array of type `T[n]` in the storage and starts its lifetime, but does not start lifetime of any of its elements.

In order to use this function in a constant expression, the allocated storage must be deallocated within the evaluation of the same expression.

After calling `allocate()` and before construction of elements, pointer arithmetic of `T*` is well-defined within the allocated array, but the behavior is undefined if elements are accessed.

