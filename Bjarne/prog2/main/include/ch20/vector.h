#pragma once
#include "vector_base.h"
#include <utility>
#include <initializer_list>

namespace ch20 {

template<typename T, typename A = std::allocator<T>>
class vector : private mem<T,A>
{
public:
    using allocator_type = mem<T,A>::allocator_type;
    using value_type = mem<T,A>::value_type;
    using size_type = mem<T,A>::size_type;
    using difference_type = mem<T,A>::difference_type;
    using reference = mem<T,A>::reference;
    using const_reference = mem<T,A>::const_reference;
    using pointer = mem<T,A>::pointer;
    using const_pointer = mem<T,A>::const_pointer;

    vector();
    ~vector();
    explicit vector(size_type, value_type = value_type());
    vector(std::initializer_list<value_type>);

    vector(const vector&) = default;
    vector& operator=(const vector&) = default;
    vector(vector&&) = default;
    vector& operator=(vector&&) = default;

    using mem<value_type, allocator_type>::operator[];

    size_type size() const { return sz; }
    using mem<T,A>::capacity;

    using mem<T,A>::reserve;
    void resize(size_type, value_type = value_type());
    void push_back(const_reference);

private:
    size_type sz;               // the number of Ts
};

template<typename T, typename A>
vector<T,A>::vector()
    : mem<T,A>(), sz {0}
{}

template<typename T, typename A>
vector<T,A>::~vector()
{}

template<typename T, typename A>
vector<T,A>::vector(size_type n, value_type val)
    : mem<T,A>(n, val), sz {n}
{}

template<typename T, typename A>
vector<T,A>::vector(std::initializer_list<value_type> list)
    : mem<T,A>(list.size()), sz {list.size()}
{
    std::copy(list.begin(), list.end(), this->data());
}

template<typename T, typename A>
void vector<T,A>::resize(size_type new_sz, value_type val)
{
    reserve(new_sz);
    if (sz < new_sz) {
        std::fill_n(this->data() + sz, new_sz - sz, val);
    } else if (new_sz < sz) {
        std::destroy(this->data() + new_sz, this->data() + sz);
    }
    sz = new_sz;
}

template<typename T, typename A>
void vector<T,A>::push_back(const_reference val)
{
    const auto space = capacity();
    if (space == 0) reserve(8);
    else if (sz == space) reserve(2 * space);

    std::fill_n(this->data() + sz, 1, val);
    ++sz;
}

}
