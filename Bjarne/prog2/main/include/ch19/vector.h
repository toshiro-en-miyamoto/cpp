#pragma once
#include <cstddef>
#include <memory>
#include <utility>
#include <initializer_list>
// for debug
#include <iostream>

namespace ch19 {

template<typename T>
class mem
{
    std::allocator<T> alloc;
    T* p;               // pointer to memory
    std::size_t n;      // capacity of the memory

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

    T& operator[](std::size_t n)
    { return *(p + n); }
    const T& operator[](std::size_t n) const
    { return *(p + n); }

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
    std::cout << "ctor " << n << '\n';
    p = alloc.allocate(n);
    std::uninitialized_copy_n(that.p, n, p);
}

template<typename T>
mem<T>& mem<T>::operator=(const mem<T>& that)
{
    if (this == &that) return *this;

    std::cout << "copy " << that.n
    << ':' << n
    << '\n';

    if (n < that.n) {
        T* new_p = alloc.allocate(that.n);
        std::uninitialized_copy_n(p, n, new_p);
        mem<T> new_mem {new_p, that.n};

        std::destroy_n(p, n);
        std::swap(new_mem, *this);
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

    std::destroy_n(p, n);
    p = std::exchange(that.p, nullptr);
    n = std::exchange(that.n, 0);

    return *this;
}

template<typename T>
void mem<T>::reserve(std::size_t new_n)
{
    if (n < new_n) {
        std::cout << "resv " << n
        << ':' << new_n
        << '\n';

        T* new_p = alloc.allocate(new_n);
        std::uninitialized_copy_n(p, n, new_p);
        mem<T> new_mem {new_p, new_n};

        std::destroy_n(p, n);
        std::swap(new_mem, *this);
    }
}

template<typename T>
class vector : protected mem<T>
{
    std::size_t sz;         // the number of Ts
public:
    vector() : sz {0}, mem<T> {} {}
    ~vector() = default;
    explicit vector(std::size_t n) : sz {n}, mem<T>(n) {}
    vector(std::initializer_list<T>);

    vector(const vector&) = default;
    vector& operator=(const vector&) = default;
    vector(vector&&) = default;
    vector& operator=(vector&&) = default;

    using mem<T>::operator[];

    std::size_t size() const { return sz; }
    using mem<T>::capacity;

    using mem<T>::reserve;
    void resize(std::size_t);
    void push_back(T);
};

template<typename T>
vector<T>::vector(std::initializer_list<T> list)
    : sz {list.size()}, mem<T>(list.size())
{
    std::copy(list.begin(), list.end(), this->elem());
}

}   // end of namespace ch19
