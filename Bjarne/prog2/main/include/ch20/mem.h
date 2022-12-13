#pragma once

#include <memory>

namespace ch20 {

template<typename T, typename A = std::allocator<T>>
struct mem
{
    using allocator_type = A;
    using value_type = T;
    using size_type = std::size_t;
    using pointer = std::allocator_traits<A>::pointer;
    using const_pointer = std::allocator_traits<A>::const_pointer;

    inline explicit mem(const size_type capacity, const allocator_type& alloc)
        : alloc_ {alloc}
        , ptr_ {nullptr}
        , capa_ {capacity}
        , size_ {0}
    {
        if (0 < capa_) ptr_ = alloc_.allocate(capa_);
    }

    inline ~mem()
    { if (0 < capa_) alloc_.deallocate(ptr_, capa_); }

    mem(const mem<T,A>&) = delete;
    mem<T,A>& operator=(const mem<T,A>&) = delete;
    mem<T,A>& operator=(mem<T,A>&&) = delete;

    inline mem(mem<T,A>&& rhs)
        : alloc_ {rhs.alloc_}
        , ptr_  {std::exchange(rhs.ptr_, nullptr)}
        , capa_ {std::exchange(rhs.capa_, 0)}
        , size_ {std::exchange(rhs.size_, 0)}
    {}

    inline void swap(mem<T,A>& rhs) noexcept
    {
        std::swap(ptr_, rhs.ptr_);
        std::swap(capa_, rhs.capa_);
        std::swap(size_, rhs.size_);
    }

    template<typename VT, typename AT>
    friend inline void swap(mem<VT,AT>& a, mem<VT,AT>& b) noexcept
    { a.swap(b); }

    allocator_type alloc_;
    pointer ptr_;
    size_type capa_;
    size_type size_;
};

}
