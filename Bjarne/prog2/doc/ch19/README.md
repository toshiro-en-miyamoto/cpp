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
 *  if sz < space, there is space for (space-sz) elements after elem[sz-1];
 */
template<typename T, typename A = std::allocator<T>>
class vector
{
    A alloc;
    T* elem;
    std::size_t sz;
    std::size_t space;
public:
    vector();
    ~vector();
    explicit vector(std::size_t, T = T());
    vector(std::initializer_list<T>);

    vector(const vector&);
    vector& operator=(const vector&);

    vector(vector&&);
    vector& operator=(vector&&);

    T& operator[](std::size_t);
    const T& operator[](std::size_t) const;
    T& at(std::size_t);
    const T& at(std::size_t) const;

    std::size_t size() const { return sz; }
    std::size_t capacity() const { return space; }

    void reserve(std::size_t);
    void resize(std::size_t, T = T());
    void push_back(const T&);
};

template<typename T, typename A>
vector<T,A>::vector()
    : alloc{}, elem{nullptr}, sz{0}, space{0}
{}

template<typename T, typename A>
vector<T,A>::~vector()
{
    std::destroy_n(elem, sz);
    alloc.deallocate(elem, space);
}

template<typename T, typename A>
vector<T,A>::vector(std::size_t size, T def)
    : alloc{}, elem{alloc.allocate(size)}
    , sz{size}, space{size}
{
    std::uninitialized_fill_n(elem, sz, def);
}

template<typename T, typename A>
vector<T,A>::vector(std::initializer_list<T> lst)
    : alloc{}, elem{alloc.allocate(lst.size())}
    , sz{lst.size()}, space{lst.size()}
{
    std::uninitialized_copy_n(lst.begin(), sz, elem);
}

template<typename T, typename A>
vector<T,A>::vector(const vector<T,A>& v)
    : alloc{}, elem{alloc.allocate(v.sz)}
    , sz{v.sz}, space{v.sz}
{
    std::uninitialized_copy_n(v.elem, sz, elem);
}

template<typename T, typename A>
vector<T,A>& vector<T,A>::operator=(const vector<T,A>& v)
{
    if (this == &v) return *this;

    if (v.sz <= space) {
        std::destroy_n(elem, sz);
        std::uninitialized_copy_n(v.elem, sz, elem);
        sz = v.sz;
    } else {
        T* p = alloc.allocate(v.sz);
        std::uninitialized_copy_n(v.elem, sz, p);

        std::destroy_n(elem, sz);
        alloc.deallocate(elem, space);

        elem = p;
        space = sz = v.sz;
    }
    return *this;
}

template<typename T, typename A>
vector<T,A>::vector(vector<T,A>&& v)
    : alloc{}, elem{v.elem}, sz{v.sz}, space{v.space}
{
    v.elem = nullptr;   // make v the empty vector
    v.sz = 0;
    v.space = 0;
}

template<typename T, typename A>
vector<T,A>& vector<T,A>::operator=(vector<T,A>&& v)
{
    if (this == v) return *this;

    std::destroy_n(elem, sz);
    alloc.deallocate(elem, space);

    elem = v.elem;      // steal v's elements
    space = sz = v.sz;

    v.elem = nullptr;   // make v the empty vector
    v.sz = 0;
    v.space = 0;
    return *this;
}

template<typename T, typename A>
T& vector<T,A>::operator[](std::size_t n)
{
    return elem[n];
}

template<typename T, typename A>
const T& vector<T,A>::operator[](std::size_t n) const
{
    return elem[n];
}

template<typename T, typename A>
T& vector<T,A>::at(std::size_t n)
{
    if (n < 0 || sz <= n) throw std::out_of_range();
    return elem[n];
}

template<typename T, typename A>
const T& vector<T,A>::at(std::size_t n) const
{
    if (n < 0 || sz <= n) throw std::out_of_range();
    return elem[n];
}

template<typename T, typename A>
void vector<T,A>::reserve(std::size_t new_space)
{
    if (new_space <= space) return;

    T* p = alloc.allocate(new_space);
    std::uninitialized_copy_n(elem, sz, p);

    std::destroy_n(elem, sz);
    alloc.deallocate(elem, space);

    elem = p;
    space = new_space;
}

template<typename T, typename A>
void vector<T,A>::resize(std::size_t new_sz, T def)
{
    reserve(new_sz);
    std::uninitialized_fill(elem + sz, elem + new_sz, def);
    std::destroy(elem + new_sz, elem + sz);
    sz = new_sz;
}

template<typename T, typename A>
void vector<T,A>::push_back(const T& val)
{
    if (space == 0) reserve(8);
    else if (sz == space) reserve(2 * space);

    std::uninitialized_fill_n(elem + sz, 1, val);
    ++sz;
}

}
```

## Resources and exceptions (§19.5)

We don’t like repetitive and ugly code because
- *repetitive* translates into code that is a maintenance hazard, and
- *ugly* translates into code that is hard to get right, hard to read, and a maintenance hazard.

We can add a `try`-block to deal with the possibility of a `throw`:

```c++
vector<int>* make_vec()
{
    vector<int>* p = new vector<int>;
    try {
        // fill the vector with data; this may throw an exception
        return p;
    } catch(...) {
        delete p;
        throw;      // re-throw
    }
}
```

This `make_vec()` function illustrates a very common style of error handling: it tries to do its job and if it can’t, it cleans up any local resources (here the vector on the free store) and indicates failure by throwing an exception. Here, the exception thrown is one that some other function (e.g., `vector::at()`) threw; `make_vec()` simply re-throws it using `throw;`. This is a simple and effective way of dealing with errors and can be used systematically.

- The purpose of the `try ... catch` code is to ensure that `make_vec()` either succeeds or throws an exception without having leaked any resources. That’s often called the *basic guarantee*.

So, `make_vec()` is a useful kind of function that obeys the basic rules for good resource management in the presence of exceptions. It provides the basic guarantee when we want to recover from exception throws. Unless something nasty is done with nonlocal data in the "fill the vector with data" part, it even provides the strong guarantee.

- If, in addition to providing the basic guarantee, a function also ensures that all observable values (all values not local to the function) are the same after failure as they were when we called the function, that function is said to provide the *strong guarantee*. The strong guarantee is the ideal when we write a function: either the function succeeded at doing everything it was asked to do or else nothing happened except that an exception was thrown to indicate failure.

However, that `try ... catch` code is still ugly. The solution is obvious: somehow we must use RAII; that is, we need to provide an object to hold that `vector<int>` so that it can delete the `vector` if an exception occurs. In `<memory>`, the standard library provides `unique_ptr` for that:

```c++
vector<int>* make_vec()
{
    std::unique_ptr<vector<int>> p {new vector<int>};
    // fill the vector with data; this may throw an exception
    return p.release();     // return tge pointer held by p
}
```

The `unique_ptr` owns the object pointed to: when the `unique_ptr` is destroyed, it deletes the object it points to. That means that if an exception is thrown while the `vector<int>` is being filled, or if we return prematurely from `make_vec`, the `vector<int>` is properly destroyed.

The `p.release()` extracts the contained pointer (to the `vector<int>`) from `p` so that we can return it, and it also makes `p` hold the `nullptr` so that destroying `p` (as is done by the return) does not destroy anything.

- The version of `make_vec()` that uses a `unique_ptr` is fine, except that it still returns a pointer, so that someone still has to remember to delete that pointer.

The technique of returning a lot of information by placing it on the free store and returning a pointer to it is very common. It is also a source of a lot of complexity and one of the major sources of memory management errors:

- Who deletes a pointer to the free store returned from a function?
- Are we sure that a pointer to an object on the free store is properly deleted in case of an exception?

Fortunately, when we added move operations to `vector`, we solved that problem for `vector`s: just use a move constructor to get the ownership of the elements out of the function. For example:

```c++
vector<int> make_vec()
{
    vector<int> res;
    // fill the vector with data; this may throw an exception
    return res;         // the move constructor efficiently transfers ownership
}
```

## RAII for `vector` (§19.5.6)

Consider `reserve()`:

```c++
template<typename T, typename A>
void vector<T,A>::reserve(std::size_t new_space)
{
    if (new_space <= space) return;

    T* p = alloc.allocate(new_space);
    std::uninitialized_copy_n(elem, sz, p);

    std::destroy_n(elem, sz);
    alloc.deallocate(elem, space);

    elem = p;
    space = new_space;
}
```

We use the standard library function `std::uninitialized_copy_n` to construct copies of the elements in `p` because it correctly handles throws from an element copy constructor and because calling a function is simpler than writing a loop.

Note that the copy operation for old elements, `std::uninitialized_copy_n`, might throw an exception. So, `p` is an example of the problem we warned about with `make_vec()`. We could apply the `unique_ptr` solution. A better solution is to step back and realize that *memory for a vector"* is a resource; that is, we can define a class `vector_base` to represent the fundamental concept we have been using all the time:

```c++
template<typename T, typename A>
struct vector_base
{
    A alloc;            // allocator
    T* elem;            // start of allocation
    std::size_t sz;     // number of elements
    std::size_t space;  // amount of allocated space

    vector_base(std::size_t n, const A& a = A())
        : alloc{a}, elem{nullptr}
        , sz{n}, space{n}
    {
        if (0 < n) elem = alloc.allocate(n);
    }

    ~vector_base()
    {
        if (elem) alloc.deallocate(elem, space);
    }
};

template<typename T, typename A = std::allocator<T>>
class vector : private vector_base<T,A>
{
public:
    // ...
};

#include <utility>  // for std::swap

template<class T, class A>
void vector<T,A>::reserve(int new_space)
{
    if (new_space <= capacity()) return;

    vector_base<T,A> b {this->alloc, new_space};
    std::uninitialized_copy_n(this->elem, this->sz, b.elem);

    std::destroy_n(this->elem, this->sz);
    this->sz = 0;
    this->space = 0;

    std::swap<vector_base<T,A>>(*this, b);
}
```

Note that `vector_base` deals with memory rather than (typed) objects. Our `vector` implementation can use that to hold objects of the desired element type. Basically, `vector` is simply a convenient interface to `vector_base`.

When we exit `reserve()`, the old allocation is automatically freed by `vector_base`’s destructor if the copy operation succeeded. If instead that exit is caused by the copy operation throwing an exception, the new allocation is freed. The `swap()` function is a standard library algorithm (from `<algorithm>`) that exchanges the value of two objects.

- We used `swap<vector_base<T,A>>(*this,b)` rather than the simpler `swap(*this,b)` because `*this` and `b` are of different types (`vector` and `vector_base`, respectively), so we had to be explicit about which swap specialization we wanted.
- Similarly, we have to explicitly use `this–>` when we refer to a member of the base class `vector_base<T,A>` from a member of the derived class `vector<T,A>`, such as `vector<T,A>::reserve()`.


Since C++20, the type parameter `T` of [`std::swap()`](https://en.cppreference.com/w/cpp/algorithm/swap) must meet the requirements of `MoveConstructible` and `MoveAssignable`. Therefore, `vector_base` must provide the move consturctor and the move assignment operator.

```c++
template<typename T, typename A>
struct vector_base
{
    A alloc;            // allocator
    T* elem;            // start of allocation
    std::size_t sz;     // number of elements
    std::size_t space;  // amount of allocated space

    vector_base()
        : alloc{}, elem{nullptr}, sz{0}, space{0}
    {}

    explicit vector_base(std::size_t n, const A& a = A())
        : alloc{a}, elem{nullptr}, sz{n}, space{n}
    {
        if (0 < n) elem = alloc.allocate(n);
    }

    ~vector_base()
    {
        if (elem) alloc.deallocate(elem, space);
    }

    vector_base(vector_base&& b)
        : alloc{b.alloc}, elem{b.elem}, sz{b.sz}, space{b.space}
    {
        b.elem = nullptr;
        b.sz = 0;
        b.space = 0;
    }

    vector_base& operator=(vector_base&& b)
    {
        if (elem) alloc.deallocate(elem, space);
        elem  = std::exchange(b.elem, nullptr);
        sz    = std::exchange(b.sz, 0);
        space = std::exchange(b.space, 0);
        return *this;
    }
};
```

`std::exchange()` of `<utility>` replaces the value of `obj` with `new_value` and returns the old value of `obj`. `T` must meet the requirements of `MoveConstructible`. Also, it must be possible to move-assign objects of type `U` to objects of type `T`.

```c++
template< class T, class U = T >
T exchange( T& obj, U&& new_value );        // since C++14
```

With `vector_base`, we can implment `vector`:

```c++
template<typename T, typename A = std::allocator<T>>
class vector : private vector_base<T,A>
{
public:
    vector();
    ~vector();
    explicit vector(std::size_t, T = T());
    vector(std::initializer_list<T>);

    vector(const vector&);
    vector& operator=(const vector&);

    vector(vector&&);
    vector& operator=(vector&&);

    T& operator[](std::size_t);
    const T& operator[](std::size_t) const;
    T& at(std::size_t);
    const T& at(std::size_t) const;

    std::size_t size() const { return this->sz; }
    std::size_t capacity() const { return this->space; }

    void reserve(std::size_t);
    void resize(std::size_t, T = T());
    void push_back(const T&);
};

template<typename T, typename A>
vector<T,A>::vector()
    : vector_base<T,A>{0}
{}

template<typename T, typename A>
vector<T,A>::~vector()
{}

template<typename T, typename A>
vector<T,A>::vector(std::size_t size, T def)
    : vector_base<T,A>{size}
{
    std::uninitialized_fill_n(this->elem, size, def);
}

template<typename T, typename A>
vector<T,A>::vector(std::initializer_list<T> lst)
    : vector_base<T,A>{lst.size()}
{
    std::uninitialized_copy_n(lst.begin(), lst.size(), this->elem);
}

template<typename T, typename A>
vector<T,A>::vector(const vector<T,A>& v)
    : vector_base<T,A>{v.sz}
{
    std::uninitialized_copy_n(v.elem, v.sz, this->elem);
}

template<typename T, typename A>
vector<T,A>& vector<T,A>::operator=(const vector<T,A>& v)
{
    if (this == &v) return *this;

    if (v.sz <= this->space) {
        std::destroy_n(this->elem, this->sz);
        std::uninitialized_copy_n(v.elem, v.sz, this->elem);
        this->sz = v.sz;
    } else {
        vector_base<T,A> base{v.sz};
        std::uninitialized_copy_n(v.elem, v.sz, base.elem);
        std::destroy_n(this->elem, this->sz);
        std::swap<vector_base<T,A>>(*this, base);
    }
    return *this;
}

template<typename T, typename A>
vector<T,A>::vector(vector<T,A>&& v)
    : vector_base<T,A>{0}
{
    std::swap<vector_base<T,A>>(*this, v);
}

template<typename T, typename A>
vector<T,A>& vector<T,A>::operator=(vector<T,A>&& v)
{
    if (this == &v) return *this;

    std::destroy_n(this->elem, this->sz);
    this->sz = 0;
    this->space = 0;

    std::swap<vector_base<T,A>>(*this, v);
    return *this;
}

template<typename T, typename A>
T& vector<T,A>::operator[](std::size_t n)
{
    return this->elem[n];
}

template<typename T, typename A>
const T& vector<T,A>::operator[](std::size_t n) const
{
    return this->elem[n];
}

template<typename T, typename A>
T& vector<T,A>::at(std::size_t n)
{
    if (n < 0 || this->sz <= n) throw std::out_of_range();
    return this->elem[n];
}

template<typename T, typename A>
const T& vector<T,A>::at(std::size_t n) const
{
    if (n < 0 || this->sz <= n) throw std::out_of_range();
    return this->elem[n];
}

template<typename T, typename A>
void vector<T,A>::reserve(std::size_t new_space)
{
    if (new_space <= this->space) return;

    vector_base<T,A> base{new_space};
    std::uninitialized_copy_n(this->elem, this->sz, base.elem);

    std::destroy_n(this->elem, this->sz);
    this->sz = 0;
    this->space = 0;

    std::swap<vector_base<T,A>>(*this, base);
}

template<typename T, typename A>
void vector<T,A>::resize(std::size_t new_sz, T def)
{
    reserve(new_sz);
    std::uninitialized_fill(
        this->elem + this->sz, this->elem + new_sz, def
    );
    std::destroy(this->elem + new_sz, this->elem + this->sz);
    this->sz = new_sz;
}

template<typename T, typename A>
void vector<T,A>::push_back(const T& val)
{
    if (this->space == 0) reserve(8);
    else if (this->sz == this->space) reserve(2 * this->space);

    std::uninitialized_fill_n(this->elem + this->sz, 1, val);
    ++(this->sz);
}
```
