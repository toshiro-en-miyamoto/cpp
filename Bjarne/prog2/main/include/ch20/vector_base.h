#pragma once

#include <memory>

#ifndef NDEBUG
#include <iostream>
#endif

namespace ch20
{

template<typename T, typename A = std::allocator<T>>
struct mem
{
    using allocator_type = A;
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = std::allocator_traits<A>::pointer;
    using const_pointer = std::allocator_traits<A>::const_pointer;

    allocator_type alloc;
    pointer ptr;                // a pointer to the memory resource
    size_type cap;              // capacity of the memory resource

    mem();
    ~mem();
    explicit mem(size_type, value_type = value_type());

    mem(const mem&);
    mem& operator=(const mem&);
    mem(mem&&);
    mem& operator=(mem&&);

    reference operator[](size_type i)
    { return *(ptr + i); }
    const_reference operator[](size_type i) const
    { return *(ptr + i); }

    pointer data()
    { return ptr; }
    const_pointer data() const
    { return ptr; }

    size_type capacity() const
    { return cap; }

    // cap < new_c ? allocate(new_c) & copy(ptr, cap, new_ptr):;
    void reserve(size_type, value_type = value_type());
};

template<typename T, typename A>
mem<T,A>::mem()
    : alloc {allocator_type()}, ptr {nullptr}, cap {0}
{
#ifndef NDEBUG
    std::cout << "new  0\n";
#endif
}

template<typename T, typename A>
mem<T,A>::~mem()
{
#ifndef NDEBUG
    std::cout << "del  " << cap << '\n';
#endif
    if (ptr) {
        std::destroy_n(ptr, cap);
        alloc.deallocate(ptr, cap);
    }
}

template<typename T, typename A>
mem<T,A>::mem(size_type n, value_type val)
    : alloc {allocator_type()}, ptr {alloc.allocate(n)}, cap {n}
{
#ifndef NDEBUG
    std::cout << "size " << n << '\n';
#endif
    std::uninitialized_fill_n(ptr, n, val);
}

template<typename T, typename A>
mem<T,A>::mem(const mem& m)
    : alloc {allocator_type()}, ptr {nullptr}, cap {m.cap}
{
#ifndef NDEBUG
    std::cout << "ctor " << m.cap << '\n';
#endif
    ptr = alloc.allocate(m.cap);
    std::uninitialized_copy_n(m.ptr, m.cap, ptr);
}

template<typename T, typename A>
mem<T,A>& mem<T,A>::operator=(const mem& m)
{
#ifndef NDEBUG
    std::cout << "copy " << m.cap << '\n';
#endif
    if (this == &m) return *this;

    if (cap < m.cap) {
        auto new_m = mem<T,A>(m.cap);
        std::uninitialized_copy_n(m.ptr, m.cap, new_m.ptr);
        std::swap(*this, new_m);
    } else {
        std::destroy_n(ptr, cap);
        std::uninitialized_copy_n(m.ptr, m.cap, ptr);
    }

    return *this;
}

template<typename T, typename A>
mem<T,A>::mem(mem&& m)
    : alloc {allocator_type()}
    , ptr {std::exchange(m.ptr, nullptr)}
    , cap {std::exchange(m.cap, 0)}
{
#ifndef NDEBUG
    std::cout << "mtor " << cap << '\n';
#endif
}

template<typename T, typename A>
mem<T,A>& mem<T,A>::operator=(mem&& m)
{
#ifndef NDEBUG
    std::cout << "move " << m.cap << '\n';
#endif
    if (this == &m) return *this;
    std::destroy_n(ptr, cap);
    ptr = std::exchange(m.ptr, nullptr);
    cap = std::exchange(m.cap, 0);
    return *this;
}

template<typename T, typename A>
void mem<T,A>::reserve(size_type new_c, value_type val)
{
#ifndef NDEBUG
    std::cout << "resv " << cap
    << ':' << new_c
    << '\n';
#endif
    if (new_c <= cap) return;

    // cap < new_c
    mem<T,A> new_m(new_c);
    std::uninitialized_copy_n(ptr, cap, new_m.ptr);
    std::uninitialized_fill_n(new_m.ptr + cap, new_c - cap, val);
    std::swap(*this, new_m);
}

}
