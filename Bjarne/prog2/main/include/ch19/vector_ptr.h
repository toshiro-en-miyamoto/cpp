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
};

template<typename T>
vector<T>& vector<T>::operator=(const vector<T>& that)
{
#ifndef NDEBUG
    std::cout << "copy " << space << '\n';
#endif
    std::unique_ptr<T[]> new_ptr {new T[space]};
    std::copy_n(that.ptr.get(), that.space, ptr.get());
    std::swap(new_ptr, ptr);
    space = that.space;
    sz = that.sz;
    return *this;
}

template<typename T>
vector<T>::vector(const vector<T>& that)
    : ptr {new T[that.space]}
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
    : ptr {new T[list.size()]}
    , space {list.size()}, sz {list.size()}
{
#ifndef NDEBUG
    std::cout << "list " << space << '\n';
#endif
    std::copy(list.begin(), list.end(), ptr.get());
}

template<typename T>
vector<T>::vector(std::size_t sp, T val)
    : ptr {new T[sp]}, space {sp}, sz {sp}
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
