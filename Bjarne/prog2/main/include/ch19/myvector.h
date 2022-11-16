#pragma once

#include <cstddef>
#include <initializer_list>
#include <memory>

namespace my {

template<typename T, typename A=std::allocator<T>>
class vector
{
    A alloc;
    std::size_t sz;
    std::size_t space;
    T* elem;
public:
    vector();
    ~vector();

    explicit vector(std::size_t);
    vector(std::initializer_list<T> lst);

    vector(const vector&);
    vector& operator=(const vector&);

    vector(vector&&);
    vector& operator=(vector&&);

    T& operator[](std::size_t i) { return elem[i]; }
    const T& operator[](std::size_t i) const { return elem[i]; }

    std::size_t size() const { return sz; }
    std::size_t capacity() const { return space; }

    void resize(std::size_t);
    void push_back(const T&);
    void reserve(std::size_t);
};

template<typename T, typename A>
vector<T,A>::vector()
    : alloc{}
    , sz{0}
    , space{0}
    , elem{nullptr}
{}

template<typename T, typename A>
vector<T,A>::~vector()
{
    // delete[] elem;
    std::destroy_n(elem, sz);
    alloc.deallocate(elem, space);
}

template<typename T, typename A>
vector<T,A>::vector(std::size_t s)
    : alloc{}
    , sz{s}
    , space{s}
    , elem{alloc.allocate(s)}
{}

template<typename T, typename A>
vector<T,A>::vector(std::initializer_list<T> lst)
    : alloc{}
    , sz{lst.size()}
    , space{lst.size()}
    , elem{alloc.allocate(lst.size())}
{
    std::copy(lst.begin(), lst.end(), elem);
}

template<typename T, typename A>
vector<T,A>::vector(const vector<T,A>& v)
    : alloc{}
    , sz{v.sz}
    , space{v.space}
    , elem{alloc.allocate(v.sz)}
{
    std::copy(v.elem, v.elem + v.sz, elem);
}

template<typename T, typename A>
vector<T,A>& vector<T,A>::operator=(const vector<T,A>& v)
{
    if (this == &v) return *this;   // self-assignment
    if (v.sz <= space) {
        std::copy(v.elem, v.elem + v.sz, elem);

        sz = v.sz;
    } else {
        // auto p = new T[v.sz];
        auto p = alloc.allocate(v.sz);

        std::copy(v.elem, v.elem + v.sz, p);

        // delete[] elem;
        std::destroy_n(elem, sz);
        alloc.deallocate(elem, space);
 
        elem = p;
        space = sz = v.sz;
    }
    return *this;
}

template<typename T, typename A>
vector<T,A>::vector(vector<T,A>&& v)
    : alloc{}
    , sz{v.sz}
    , elem{v.elem}
    , space{v.space}
{
    v.sz = 0;           // make v the empty vector
    v.elem = nullptr;
    v.space = 0;
}

template<typename T, typename A>
vector<T,A>& vector<T,A>::operator=(vector<T,A>&& v)
{
    // delete[] elem;
    std::destroy_n(elem, sz);
    alloc.deallocate(elem, space);

    elem = v.elem;      // steal v's elements
    space = sz = v.sz;

    v.sz = 0;           // make v the empty vector
    v.elem = nullptr;
    v.space = 0;
    return *this;
}

template<typename T, typename A>
void vector<T,A>::resize(std::size_t new_size)
{
    reserve(new_size);
    sz = new_size;
}

template<typename T, typename A>
void vector<T,A>::push_back(const T& d)
{
    if (space == 0) {
        reserve(8);
    } else if (sz == space) {
        reserve(2 * space);
    }
    elem[sz] = d;
    ++sz;
}

template<typename T, typename A>
void vector<T,A>::reserve(std::size_t new_space)
{
    if (new_space <= space) return;

    //auto p = new T[new_space];
    auto p = alloc.allocate(new_space);

    std::copy(elem, elem + new_space, p);

    // delete[] elem;
    std::destroy_n(elem, sz);
    alloc.deallocate(elem, space);

    elem = p;
    space = new_space;
}

}
