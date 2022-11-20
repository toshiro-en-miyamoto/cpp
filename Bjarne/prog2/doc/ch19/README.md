# Vector, Templates, and Exceptions

## Changing size (§19.2)

```c++
class vector
{
    int sz;
    double* elem;
    int space;
public:
    vector() : sz{0}, elem{nullptr}, space[0] {}
    ~vector() { delete[] elem; }
    explicit vector(int);

    vector(const vector&);
    vector& operator=(const vector&);

    vector(vector&&);
    vector& operator=(vector&&);

    double& operator[](int n) { return elem[n]; }
    const double& operator[](int n) const { return elem[n]; }

    int size() const { return sz; }
    int capacity() const { return space; }

    void reserve(int);
    void resize(int);
    void push_back(double d);
};

explicit vector::vector(int s)
    : sz{s}
    , elem{new double[s]}
    , space{s}
{
    for (int i = 0; i < s; ++i) {
        elem[i] = 0.0;      // initialize new elements
    }
}
```

The `vector::reserve()` adds space for new elements:

```c++
void vector::reserve(int new_space)
{
    if (new_space <= space) return;

    double* p = new double[new_size];
    for (int i = 0; i < sz; ++i) {
        p[i] = elem[i];     // copy elements
    }
    delete[] elem;          // destroy old elemenets and
                            // deallocate old space
    elem = p;
    space = new_space;
}
```

Note that we don’t initialize the elements of the reserved space. After all, we are just reserving space; using that space for elements is the job of `push_back()` and `resize()`.

The `vector::resize()` makes the vector have `new_size` elements. The operation initializes each new element with default value.

```c++
void vector::resize(int new_size)
{
    reserve(new_size);
    for (int i = sz; i < new_size; ++i) {
        elem[i] = 0.0;      // initialize new elements
    }
    sz = new_size;
}
```

Given `reserve()`, `push_back()` is quite siple:

```c++
void vector::push_back(double d)
{
    if (space == 0)
        reserve(8);
    else if (sz == space)
        reserve(2 * space);

    elem[sz] = d;           // copy the argument
    ++sz;
}
```

After assignment `v1 = v2`, the vector `v1` should be a copy of `v2`.

```c++
vector& vector::operator=(const vector& v)
{
    if (this == v) return *this;

    if (v.sz <= space) {                    // enough space
        for (int i = 0; i < v.sz; ++i) {
            elem[i] = v.elem[i];            // copy elements
        }
        // need to destroy old elements?
        sz = v.sz;
    } else {                                // space shortage
        double* p = new double[v.sz];
        for (int i = 0; i < v.sz; ++i) {
            p[i] = v.elem[i];               // copy elements
        }
        delete[] elem;      // destroy old elemenets and
                            // deallocate old space
        elem = p;                           // p := this
        space = sz = v.sz;
    }
    return *this;
}
```

## Templates (§19.3)

*Generic programming*: Writing code that works with a variety of types presented as arguments, as long as those argument types meet specific syntactic and semantic requirements.

This form of generic programming relying on explicit template parameters is often called *parametric polymorphism*.

In contrast, the polymorphism you get from using class hierarchies and virtual functions is called *ad hoc polymorphism* and that style of programming is called *object-oriented programming*.

There are several differences between object-oriented programming (using class hierarchies and virtual functions) and generic programming (using templates). The most obvious is that the choice of function invoked:

- when you use generic programming, it is determined by the compiler at compile time,
- whereas for object-oriented programming, it is not determined until run time.

To sum up:

- Generic programming: supported by templates, relying on compile-time resolution
- Object-oriented programming: supported by class hierarchies and virtual functions, relying on run-time resolution

## Generalizing `vector` (§19.3.7)

We must review the definitions of `push_back()`, `resize()`, and `reserve()` because they make assumptions that are true for `double`s, but not true for all types that we’d like to use as `vector` element types:

- How do we handle a `vector<X>` where `X` doesn’t have a default value?
- How do we ensure that elements are destroyed when we are finished with them?

We can handle types without a default by giving the user the option to specify the value to be used when we need a *default value*:

```c++
template<typename T>
explicit vector<T>::vector(int s, T def = T());

template<typename T>
void vector<T>::resize(int new_size, T def = T());
```

The is, use `T()` as the default value unless the user says otherwise:

```c++
struct X
{                       // no default constructor
    X(int);             // the only constructor
};

vector<X> v2(10);       // error: tries to make 10 instances of X()

vector<X> v3;
v3.resize(100, X(2));   // OK: add 100 copies of X(2)
v3.resize(200);         // error: tries to add 100 instances of X()
```

The destructor problem is harder to address. Basically, we need to deal with something really awkward: a data structure consisting of some initialized data and some uninitialized data.

First, we need to find a way of getting and manipulating uninitialized storage. Fortunately, the standard library provides a class `std::allocator`, which provides uninitialized memory.

- Allocate memory of a size suitable to hold an object of type `T` without initializing
- Construct an object of type `T` in uninitialized space
- Destroy an object of type `T`, thus returning its space to the uninitialized state
- Deallocate uninitialized space of a size suitable for an object of type `T`

Note that `std::allocator` member functions `construct()` and `desctroy()` were deprecated in C++17. Instead, the following *uninitialized memory algorithms* are available in `<memory>` since C++17:

- `uninitialized_copy()`: copies a range of objects to an uninitialized area of memory
- `uninitialized_copy_n()`: (C++11)	copies a number of objects to an uninitialized area of memory
- `uninitialized_fill()`: copies an object to an uninitialized area of memory, defined by a range
- `uninitialized_fill_n()`: copies an object to an uninitialized area of memory, defined by a start and a count
- `destroy()`: (C++17) destroys a range of objects
- `destroy_n()`: (C++17) destroys a number of objects in a range

```c++
#pragma once

#include <cstddef>
#include <initializer_list>
#include <memory>

namespace ch19 {

/**
 * Uses std::allocator to allocate/deallocate memory for elements.
 * 
 * Invariant:
 *  if 0 ≤ n < sz, elem[n] is element n;
 *  sz ≤ space;
 *  if sz < space, there is space for (space - sz) doubles after elem[sz - 1];
 */
template<typename T, typename A = std::allocator<T>>
class vector {
    A alloc;
    std::size_t sz;
    std::size_t space;
    T* elem;
public:
    vector() : alloc{}, sz{0}, space{0}, elem{nullptr} {}
    ~vector();
    vector(std::size_t, T = T());
    vector(std::initializer_list<T>);

    vector(const vector&);
    vector& operator=(const vector&);

    vector(vector&&);
    vector& operator=(vector&&);

    T& operator[](std::size_t n) { return elem[n]; }
    const T& operator[](std::size_t n) const { return elem[n]; }

    std::size_t size() const { return sz; }
    std::size_t capacity() const { return space; }

    void reserve(std::size_t);
    void resize(std::size_t, T = T());
    void push_back(const T&);
};

template<typename T, typename A>
vector<T,A>::~vector()
{
    // for (auto i = 0; i < sz; ++i) alloc.destroy(&elem[i]);
    std::destroy_n(elem, sz);
    alloc.deallocate(elem, space);
}

template<typename T, typename A>
vector<T,A>::vector(std::size_t size, T def)
    : alloc{}
    , sz{size}
    , space{size}
    , elem{alloc.allocate(size)}
{
    // for (auto i = 0; i < size; ++i) alloc.construct(&elem[i], def);
    std::uninitialized_fill_n(elem, sz, def);
}

template<typename T, typename A>
vector<T,A>::vector(std::initializer_list<T> lst)
    : alloc{}
    , sz{lst.size()}
    , space{lst.size()}
    , elem{alloc.allocate(lst.size())}
{
    // auto data = std::data(lst);
    // for (auto i = 0; i < sz; ++i) alloc.construct(&elem[i], data[i]);
    std::uninitialized_copy_n(lst.begin(), sz, elem);
}

template<typename T, typename A>
vector<T,A>::vector(const vector<T,A>& v)
    : alloc{}
    , sz{v.sz}
    , space{v.sz}
    , elem{alloc.allocate(v.sz)}
{
    // for (auto i = 0; i < v.sz; ++i) alloc.construct(&elem[i], v.elem[i]);
    std::uninitialized_copy_n(v.elem, sz, elem);
}

template<typename T, typename A>
vector<T,A>& vector<T,A>::operator=(const vector<T,A>& v)
{
    if (this == &v) return *this;

    if (v.sz <= space) {
        // for (auto i = 0; i < sz; ++i) alloc.destroy(&elem[i]);
        std::destroy_n(elem, sz);
        // for (auto i = 0; i < v.sz; ++i) alloc.construct(&elem[i], v.elem[i]);
        std::uninitialized_copy_n(v.elem, sz, elem);
        sz = v.sz;
    } else {
        T* p = alloc.allocate(v.sz);
        // for (auto i = 0; i < v.sz; ++i) alloc.construct(&p[i], v.elem[i]);
        std::uninitialized_copy_n(v.elem, sz, p);
        // for (auto i = 0; i < sz; ++i) alloc.destroy(&elem[i]);
        std::destroy_n(elem, sz);
        alloc.deallocate(elem, space);

        space = sz = v.sz;
        elem = p;
    }
    return *this;
}

template<typename T, typename A>
vector<T,A>::vector(vector<T,A>&& v)
    : alloc{}
    , sz{v.sz}
    , space{v.space}
    , elem{v.elem}
{
    v.sz = 0;           // make v the empty vector
    v.space = 0;
    v.elem = nullptr;
}

template<typename T, typename A>
vector<T,A>& vector<T,A>::operator=(vector<T,A>&& v)
{
    if (this == v) return *this;

    // destroy old elements, deallocate old space
    // for (auto i = 0; i < sz; ++i) alloc.destroy(&elem[i]);
    std::destroy_n(elem, sz);
    alloc.deallocate(elem, space);

    elem = v.elem;      // steal v's elements
    space = sz = v.sz;

    v.sz = 0;           // make v the empty vector
    v.space = 0;
    v.elem = nullptr;
    return *this;
}

template<typename T, typename A>
void vector<T,A>::reserve(std::size_t new_space)
{
    if (new_space <= space) return;

    // double* p = new T[new_size];
    T* p = alloc.allocate(new_space);
    // for (int i = 0; i < sz; ++i) { p[i] = elem[i]; }
    // for (auto i = 0; i < sz; ++i) alloc.construct(&p[i], elem[i]);
    std::uninitialized_copy_n(elem, sz, p);

    // delete[] elem;
    // for (auto i = 0; i < sz; ++i) alloc.destroy(&elem[i]);
    std::destroy_n(elem, sz);
    alloc.deallocate(elem, space);

    space = new_space;
    elem = p;
}

template<typename T, typename A>
void vector<T,A>::resize(std::size_t new_sz, T def)
{
    reserve(new_sz);

    // for (int i = sz; i < new_size; ++i) { elem[i] = 0.0; }
    // for (auto i = sz; i < new_sz; ++i) alloc.construct(&elem[i], def);
    std::uninitialized_fill(elem + sz, elem + new_sz, def);

    // vector of double didn't care about [new_size, sz)
    // for (auto i = new_sz; i < sz; ++i) alloc.destroy(&elem[i]);
    std::destroy(elem + new_sz, elem + sz);

    sz = new_sz;
}

template<typename T, typename A>
void vector<T,A>::push_back(const T& val)
{
    if (space == 0) reserve(8);
    else if (sz == space) reserve(2 * space);

    // elem[sz] = val;
    // alloc.construct(&elem[sz], val);
    std::uninitialized_fill_n(elem + sz, 1, val);
    ++sz;
}

}
```

## Range checking and exceptions (§19.4)
