#pragma once
#include <cstddef>
#include <memory>
#include <utility>
// for debug
#include <iostream>

namespace ch19 {

/*!
A memory resource handler.
Type T must define the default constructor.

        mem& operator=(const mem&);    void reserve(std::size_t);

n < a.n 
        np = allocate(a.n)
        uninit_copy_n(a.p, a.n, np)
        destroy_n(p, n)
        deallocate(p, n)
        p = np
        n = a.n

else
        destory_n(p, n)                 nop
        uninit_copy_n(a.p, a.n, p)

*/
template<typename T>
class mem
{
    std::allocator<T> alloc;
    T* p;               // pointer to memory
    std::size_t n;      // the number of Ts

    mem(T* p, std::size_t);
public:
    mem();
    ~mem();
    explicit mem(std::size_t);
    mem(const mem&);
    mem& operator=(const mem&);
    mem(mem&&);
    mem& operator=(mem&&);

    T* elem() { return p; }
    const T* elem() const { return p; }

    void reserve(std::size_t);
    std::size_t capacity() const { return n; }
};

template<typename T>
mem<T>::mem(T* p, std::size_t n)
    : alloc {}, p {p}, n {n}
{
    std::cout << "new  " << n << '\n';
}
 
template<typename T>
mem<T>::mem()
    : alloc {}, p {nullptr}, n {0}
{
    std::cout << "new  0\n";
}

template<typename T>
mem<T>::~mem()
{
    std::cout << "del  " << n << '\n';
    alloc.deallocate(p, n);
}

template<typename T>
mem<T>::mem(std::size_t n)
    : alloc {}, p {nullptr}, n {n}
{
    std::cout << "size " << n << '\n';
    p = alloc.allocate(n);
    std::uninitialized_fill_n(p, n, T {});
}

template<typename T>
mem<T>::mem(const mem<T>& that)
    : alloc {}, p {nullptr}, n {that.n}
{
    p = alloc.allocate(n);
    std::uninitialized_copy_n(that.p, n, p);
}

template<typename T>
mem<T>& mem<T>::operator=(const mem<T>& that)
{
    if (this == &that) return *this;

    if (n < that.n) {
        T* new_p = alloc.allocate(that.n);
        std::uninitialized_copy_n(p, n, new_p);

        std::destroy_n(p, n);
        alloc.deallocate(p, n);

        p = new_p;
        n = that.n;
    } else {
        std::destroy_n(p, n);
        std::uninitialized_copy_n(that.p, that.n, p);
    }

    return *this;
}

template<typename T>
mem<T>::mem(mem<T>&& that)
    : alloc {}, p {that.p}, n {that.n}
{
    std::cout << "mtor " << n << '\n';
    that.p = nullptr;
    that.n = 0;
}

template<typename T>
mem<T>& mem<T>::operator=(mem<T>&& that)
{
    std::cout << "move " << that.n << '\n';
    if (this == &that) return *this;

    // std::destroy_n(p, n);
    p = std::exchange(that.p, nullptr);
    n = std::exchange(that.n, 0);

    return *this;
}

template<typename T>
void mem<T>::reserve(std::size_t new_n)
{
    if (n < new_n) {
        std::cout << "reserving from " << n
        << " to " << new_n
        << '\n';

        T* new_p = alloc.allocate(new_n);
        std::uninitialized_copy_n(p, n, new_p);
        mem<T> new_mem {new_p, new_n};

        std::destroy_n(p, n);
        std::swap(new_mem, *this);
    }
    std::cout << "reserved\n";
}

}   // end of namespace ch19
