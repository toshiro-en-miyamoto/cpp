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

    vector(const vector<T>&);
    vector<T>& operator=(const vector<T>&);
    vector(vector<T>&&) noexcept;
    vector<T>& operator=(vector<T>&&) noexcept;

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

    void reserve(size_type);
    void push_back(const_reference);
private:
    void swap(vector<T>&) noexcept;

    template<typename VT>
    friend void swap(vector<VT>& ,vector<VT>&) noexcept;

    std::unique_ptr<T[]> ptr_;
    size_type capa_;        // allocated memory
    size_type size_;        // number of elements
};

template<typename T>
vector<T>& vector<T>::operator=(vector<T>&& rhs) noexcept
{
    if (this == &rhs) return *this;

    vector<T> temp(std::move(*this));
    rhs.swap(*this);

    return *this;
}

template<typename T>
vector<T>::vector(vector<T>&& rhs) noexcept
    : ptr_  {std::exchange(rhs.ptr_, nullptr)}
    , capa_ {std::exchange(rhs.capa_, 0)}
    , size_ {std::exchange(rhs.size_, 0)}
{}

template<typename T>
vector<T>& vector<T>::operator=(const vector<T>& rhs)
{
    if (this == &rhs) return *this;

    if (capa_ < rhs.capa_) {
        vector<T> copy(rhs);
        copy.swap(*this);
    } else {
        std::copy(rhs.begin(), rhs.end(), begin());
        size_ = rhs.size_;
    }

    return *this;
}

template<typename T>
vector<T>::vector(const vector<T>& rhs)
    : ptr_ {nullptr}, capa_ {0}, size_ {0}
{
    if ( 0 < rhs.capa_) {
        vector<T> temp(rhs.capa_);
        std::copy(rhs.begin(), rhs.end(), temp.begin());
        temp.size_ = rhs.size_;
        temp.swap(*this);
    }
}

template<typename T>
void vector<T>::push_back(const_reference val)
{
    if (capa_ == 0) reserve(4);
    else if (size_ == capa_) reserve(2 * capa_);

    std::fill_n(end(), 1, val);
    ++size_;
}

template<typename T>
void vector<T>::reserve(size_type capacity)
{
    if (capa_ < capacity) {
        vector<T> lhs(capacity);
        std::copy(begin(), end(), lhs.begin());
        lhs.size_ = size_;
        lhs.swap(*this);
    } else if (capacity < size_) {
        size_ = capacity;
    }
}

template<typename T>
vector<T>::vector(std::initializer_list<value_type> list)
    : ptr_ {nullptr}, capa_ {0}, size_ {0}
{
    auto new_capa = list.size();
    if (0 < new_capa) {
        vector<T> v {new_capa};
        std::copy(list.begin(), list.end(), v.begin());
        v.size_ = new_capa;
        v.swap(*this);
    }
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
