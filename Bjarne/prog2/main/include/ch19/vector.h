#include <cstddef>
#include <initializer_list>
#include <memory>
#include <utility>
#include <iostream>

namespace ch19 {

template<typename T, typename A>
struct vector_base
{
    A alloc;            // allocator
    T* elem;            // start of allocation
    std::size_t sz;     // number of elements
    std::size_t space;  // amount of allocated space

    vector_base()
        : alloc{}, elem{nullptr}, sz{0}, space{0}
    {}

    explicit vector_base(std::size_t n, const A& a = A())
        : alloc{a}, elem{nullptr}, sz{n}, space{n}
    {
        if (0 < n) {
            elem = alloc.allocate(n);
            std::cout
            << "new  (" << elem
            << ", " << sz
            << ", " << space
            << ")\n";
        }
    }

    ~vector_base()
    {
        if (elem) {
            alloc.deallocate(elem, space);
            std::cout
            << "gone (" << elem
            << ", " << sz
            << ", " << space
            << ")\n";
        }
    }

    vector_base(vector_base&& b)
        : alloc{b.alloc}, elem{b.elem}
        , sz{b.sz}, space{b.space}
    {
        b.elem = nullptr;
        b.sz = 0;
        b.space = 0;
    }

    vector_base& operator=(vector_base&& b)
    {
        if (elem) alloc.deallocate(elem, space);
        elem  = std::exchange(b.elem,  nullptr);
        sz    = std::exchange(b.sz,    0);
        space = std::exchange(b.space, 0);
        return *this;
    }
};

template<typename T, typename A = std::allocator<T>>
class vector : private vector_base<T,A>
{
public:
    vector();
    ~vector();
    explicit vector(std::size_t, T = T());
    vector(std::initializer_list<T>);

    vector(const vector&);
    vector& operator=(const vector&);

    vector(vector&&);
    vector& operator=(vector&&);

    T& operator[](std::size_t);
    const T& operator[](std::size_t) const;
    T& at(std::size_t);
    const T& at(std::size_t) const;

    std::size_t size() const { return this->sz; }
    std::size_t capacity() const { return this->space; }

    void reserve(std::size_t);
    void resize(std::size_t, T = T());
    void push_back(const T&);
};

template<typename T, typename A>
vector<T,A>::vector()
    : vector_base<T,A>{0}
{}

template<typename T, typename A>
vector<T,A>::~vector()
{}

template<typename T, typename A>
vector<T,A>::vector(std::size_t size, T def)
    : vector_base<T,A>{size}
{
    std::uninitialized_fill_n(this->elem, size, def);
}

template<typename T, typename A>
vector<T,A>::vector(std::initializer_list<T> lst)
    : vector_base<T,A>{lst.size()}
{
    std::uninitialized_copy_n(lst.begin(), lst.size(), this->elem);
}

template<typename T, typename A>
vector<T,A>::vector(const vector<T,A>& v)
    : vector_base<T,A>{v.sz}
{
    std::uninitialized_copy_n(v.elem, v.sz, this->elem);
}

template<typename T, typename A>
vector<T,A>& vector<T,A>::operator=(const vector<T,A>& v)
{
    if (this == &v) return *this;

    if (v.sz <= this->space) {
        std::destroy_n(this->elem, this->sz);
        std::uninitialized_copy_n(v.elem, v.sz, this->elem);
        this->sz = v.sz;
    } else {
        vector_base<T,A> base{v.sz};
        std::uninitialized_copy_n(v.elem, v.sz, base.elem);
        std::destroy_n(this->elem, this->sz);
        std::swap<vector_base<T,A>>(*this, base);
    }
    return *this;
}

template<typename T, typename A>
vector<T,A>::vector(vector<T,A>&& v)
    : vector_base<T,A>{0}
{
    std::swap<vector_base<T,A>>(*this, v);
}

template<typename T, typename A>
vector<T,A>& vector<T,A>::operator=(vector<T,A>&& v)
{
    if (this == &v) return *this;

    std::destroy_n(this->elem, this->sz);
    this->sz = 0;
    this->space = 0;

    std::swap<vector_base<T,A>>(*this, v);
    return *this;
}

template<typename T, typename A>
T& vector<T,A>::operator[](std::size_t n)
{
    return this->elem[n];
}

template<typename T, typename A>
const T& vector<T,A>::operator[](std::size_t n) const
{
    return this->elem[n];
}

template<typename T, typename A>
T& vector<T,A>::at(std::size_t n)
{
    if (n < 0 || this->sz <= n) throw std::out_of_range();
    return this->elem[n];
}

template<typename T, typename A>
const T& vector<T,A>::at(std::size_t n) const
{
    if (n < 0 || this->sz <= n) throw std::out_of_range();
    return this->elem[n];
}

template<typename T, typename A>
void vector<T,A>::reserve(std::size_t new_space)
{
    if (new_space <= this->space) return;

    vector_base<T,A> base{new_space};
    std::uninitialized_copy_n(this->elem, this->sz, base.elem);

    std::destroy_n(this->elem, this->sz);
    this->sz = 0;
    this->space = 0;

    std::swap<vector_base<T,A>>(*this, base);
}

template<typename T, typename A>
void vector<T,A>::resize(std::size_t new_sz, T def)
{
    reserve(new_sz);
    std::uninitialized_fill(
        this->elem + this->sz, this->elem + new_sz, def
    );
    std::destroy(this->elem + new_sz, this->elem + this->sz);
    this->sz = new_sz;
}

template<typename T, typename A>
void vector<T,A>::push_back(const T& val)
{
    if (this->space == 0) reserve(8);
    else if (this->sz == this->space) reserve(2 * this->space);

    std::uninitialized_fill_n(this->elem + this->sz, 1, val);
    ++(this->sz);
}

}
