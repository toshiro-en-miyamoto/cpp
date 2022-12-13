#pragma once

#include <memory>
#include <initializer_list>

namespace ch20 {

template<typename T>
class vector
{
public:
    using value_type = T;
    using size_type = std::size_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = std::unique_ptr<T[]>::pointer;
    using const_pointer = const pointer;
    using iterator = pointer;
    using const_iterator = const_pointer;

    explicit vector(size_type capacity);
    vector(std::initializer_list<value_type>);
    ~vector() = default;

    inline size_type capacity() const noexcept
    { return capa_; }
    inline size_type size() const noexcept
    { return size_; }

    inline reference operator[](size_type i) noexcept
    { return ptr_[i]; }
    inline const_reference operator[](size_type i) const noexcept
    { return ptr_[i]; }
    inline pointer data() noexcept
    { return ptr_.get(); };
    inline const_pointer data () const noexcept
    { return ptr_.get(); }

    inline iterator begin()
    { return ptr_.get(); }
    inline const_iterator begin() const
    { return ptr_.get(); }
    inline iterator end()
    { return ptr_.get() + size_; }
    inline const_iterator end() const
    { return ptr_.get() + size_; }

private:
    void swap(vector<T>&) noexcept;

    template<typename VT>
    friend void swap(vector<VT>& ,vector<VT>&) noexcept;

    std::unique_ptr<T[]> ptr_;
    size_type capa_;        // allocated memory
    size_type size_;        // number of elements
};

template<typename T>
vector<T>::vector(std::initializer_list<value_type> list)
    : ptr_ {nullptr}, capa_ {list.size()}
{
    if (0 < capa_) {
        auto p = std::make_unique<value_type[]>(capa_);
        std::copy(list.begin(), list.end(), p.get());
        ptr_ = std::move(p);
    }
    size_ = capa_;
}

template<typename T>
vector<T>::vector(size_type capacity)
    : ptr_ {nullptr}, capa_ {capacity}, size_ {0}
{
    if (0 < capa_) {
        auto p = std::make_unique<value_type[]>(capa_);
        ptr_ = std::move(p);
    }
}

template<typename VT>
void swap(vector<VT>& a,vector<VT>& b) noexcept
{
    a.swap(b);
}

template<typename T>
void vector<T>::swap(vector<T>& rhs) noexcept
{
    std::swap(ptr_,  rhs.ptr_);
    std::swap(capa_, rhs.capa_);
    std::swap(size_, rhs.size_);
}

}
