#pragma once

#include <memory>
#include <algorithm>
#include <initializer_list>
// for debug
#include <iostream>

namespace ch19
{

template<typename T>
class vector
{
    std::unique_ptr<T[]> ptr;
    std::size_t space;
    std::size_t sz;
public:
    vector();
    ~vector();
    explicit vector(std::size_t, T = T());
    vector(std::initializer_list<T>);

    std::size_t capacity() const { return space; }
    std::size_t size() const { return sz; }

    T& operator[](std::size_t);
    const T& operator[](std::size_t) const;

    vector(const vector<T>&);
    vector& operator=(const vector<T>&);
    vector(vector<T>&&);
    vector& operator=(vector<T>&&);

    void reserve(std::size_t);
    void resize(std::size_t, T = T());
    void push_back(const T&);
};

template<typename T>
void vector<T>::push_back(const T& val)
{
    if (space == 0) reserve(8);
    else if (sz == space) reserve(2 * space);
    std::fill_n(ptr.get() + sz, 1, val);
    ++sz;
}

template<typename T>
void vector<T>::resize(std::size_t new_sz, T val)
{
    if (sz == new_sz) return;

    reserve(new_sz);
    if (sz < new_sz) {
        std::fill_n(ptr.get() + sz, new_sz - sz, val);
    // } else {
        // new_sz < sz

        // not need to destroy here
        // unique_ptr<T[]> destory all elements in T[]

        // std::destroy_n(ptr.get() + new_sz, sz - new_sz);
    }
    sz = new_sz;
}

template<typename T>
void vector<T>::reserve(std::size_t new_space)
{
#ifndef NDEBUG
    std::cout << "resv " << new_space << '\n';
#endif
    if (space < new_space) {
        auto new_ptr = std::make_unique<T[]>(new_space);
        std::copy_n(ptr.get(), space, new_ptr.get());
        std::swap(ptr, new_ptr);
        space = new_space;
    }
}

template<typename T>
vector<T>& vector<T>::operator=(vector<T>&& that)
{
#ifndef NDEBUG
    std::cout << "move " << that.space << '\n';
#endif
    ptr = std::move(that.ptr);
    space = std::exchange(that.space, 0);
    sz = std::exchange(that.sz, 0);
    return *this;
}

template<typename T>
vector<T>::vector(vector<T>&& that)
    : ptr {std::move(that.ptr)}
    , space {std::exchange(that.space, 0)}
    , sz {std::exchange(that.sz, 0)}
{
#ifndef NDEBUG
    std::cout << "mtor " << space << '\n';
#endif
}

template<typename T>
vector<T>& vector<T>::operator=(const vector<T>& that)
{
#ifndef NDEBUG
    std::cout << "copy " << that.space << '\n';
#endif
    std::unique_ptr<T[]> new_ptr {std::make_unique<T[]>(that.space)}; // new T[space]
    std::copy_n(that.ptr.get(), that.space, new_ptr.get());
    std::swap(new_ptr, ptr);
    space = that.space;
    sz = that.sz;
    return *this;
}

template<typename T>
vector<T>::vector(const vector<T>& that)
    : ptr {std::make_unique<T[]>(that.space)}
    , space {that.space}, sz {that.sz}
{
#ifndef NDEBUG
    std::cout << "ctor " << space << '\n';
#endif
    std::copy_n(that.ptr.get(), space, ptr.get());
}

template<typename T>
const T& vector<T>::operator[](std::size_t i) const
{
    return ptr[i];
}

template<typename T>
T& vector<T>::operator[](std::size_t i)
{
    return ptr[i];
}

template<typename T>
vector<T>::vector(std::initializer_list<T> list)
    : ptr {std::make_unique<T[]>(list.size())}
    , space {list.size()}, sz {list.size()}
{
#ifndef NDEBUG
    std::cout << "list " << space << '\n';
#endif
    std::copy(list.begin(), list.end(), ptr.get());
}

template<typename T>
vector<T>::vector(std::size_t sp, T val)
    : ptr {std::make_unique<T[]>(sp)}
    , space {sp}, sz {sp}
{
#ifndef NDEBUG
    std::cout << "size " << space << '\n';
#endif
    std::fill_n(ptr.get(), sp, val);
}

template<typename T>
vector<T>::~vector()
{
#ifndef NDEBUG
    std::cout << "del  " << space << '\n';
#endif
}

template<typename T>
vector<T>::vector()
    : ptr {}, space {0}, sz {0}
{
#ifndef NDEBUG
    std::cout << "new  0\n";
#endif
}

}
