#pragma once

#include <cstddef>
#include <initializer_list>
#include <memory>

namespace ch19 {

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
