#include <memory>
#include <initializer_list>

namespace ch20 {

template<typename T, typename A = std::allocator<T>>
class mem
{
public:
    using allocator_type = A;
    using value_type = T;
    using size_type = std::size_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = std::allocator_traits<A>::pointer;
    using const_pointer = std::allocator_traits<A>::const_pointer;
    using iterator = pointer;
    using const_iterator = const_pointer;

    explicit mem(size_type capacity);
    mem(std::initializer_list<value_type>);
    ~mem();

    mem(const mem<T,A>&);
    mem(mem<T,A>&&) noexcept;
    mem<T,A>& operator=(mem<T,A>);

    size_type capacity() const noexcept { return capa_; }

    reference operator[](size_type i) noexcept
    { return *(ptr_ + i); }
    const_reference operator[](size_type i) const noexcept
    { return *(ptr_ + i); }

private:
    void swap(mem<T,A>&) noexcept;

    template<typename VT, typename AT>
    friend void swap(mem<VT,AT>&, mem<VT,AT>&) noexcept;

    allocator_type alloc;
    pointer ptr_;
    size_type capa_;
};

template<typename T, typename A>
mem<T,A>& mem<T,A>::operator=(mem<T,A> rhs)
{
    if (this == &rhs) return *this;
    rhs.swap(*this);
    return *this;
}

template<typename T, typename A>
mem<T,A>::mem(mem<T,A>&& rhs) noexcept
    : alloc {}
    , ptr_  {std::exchange(rhs.ptr_, nullptr)}
    , capa_ {std::exchange(rhs.capa_, 0)}
{}

template<typename T, typename A>
mem<T,A>::mem(const mem<T,A>& rhs)
    : alloc {}, ptr_ {nullptr}, capa_ {rhs.capa_}
{
    if (0 < capa_) ptr_ = alloc.allocate(capa_);
}

template<typename VT, typename AT>
void swap(mem<VT,AT>& a, mem<VT,AT>& b)
{
    a.swap(b);
}

template<typename T, typename A>
void mem<T,A>::swap(mem<T,A>& rhs) noexcept
{
    std::swap(ptr_, rhs.ptr_);
    std::swap(capa_, rhs.capa_);
}

template<typename T, typename A>
mem<T,A>::mem(std::initializer_list<value_type> list)
    : allo {}, ptr_ {nullptr}, capa_ {list.isze()}
{
    if (0 < capa_) ptr_ = alloc.allocate(capa_);
}

template<typename T, typename A>
mem<T,A>::~mem()
{
    if (0 < capa_) alloc.deallocate(ptr_, capa_);
}

template<typename T, typename A>
mem<T,A>::mem(size_type capacity)
    : alloc {}, ptr_ {nullptr}, capa_ {capacity}
{
    if (0 < capa_) ptr_ = alloc.allocate(capa_);
}

}
