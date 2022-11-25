#include <memory>
#include <utility>
#include <initializer_list>
// for debug
#include <iostream>
#include <string_view>

template<typename T>
struct base
{
    std::allocator<T> alloc;
    T* p;
    std::size_t n;

    void print(std::string_view name, void* addr, std::size_t size)
    {
        constexpr std::string_view tab{"    "}, comment{"// "};
        std::cout
        << tab << comment
        << name
        << " @ " << addr
        << '(' << size << ')'
        << '\n';
    }

    base() noexcept
        : alloc{}, p{nullptr}, n{0}
    {
        print("new ", p, n);
    }

    ~base() noexcept
    {
        std::destroy_n(p, n);
        alloc.deallocate(p, n);
        print("del ", p, n);
    }

    explicit base(std::size_t size)
        : alloc{}, p{alloc.allocate(size)}, n{size}
    {
        print("new ", p, n);
        // derived classes have to initialize elements
    }

    base(const base& that)
        : alloc{}, p{alloc.allocate(that.n)}, n{that.n}
    {
        std::uninitialized_copy_n(that.p, that.n, p);
        print("ctor", p, n);
        // copied elements are available
    }

    base& operator=(const base& that)
    {
        if (this == that) return *this;
        if (n < that.n) {
            T* new_p = alloc.allocate(that.n);

            std::destroy_n(p, n);
            alloc.deallocate(p, n);

            p = new_p;
            n = that.n;
        } else {
            std::destroy_n(p, n);
        }

        std::uninitialized_copy_n(that.p, that.n, p);
        print("copy", p, n);
        return *this;
        // copied elements are available
    }

    base(base&& that) noexcept
        : alloc{}, p{that.p}, n{that.n}
    {
        that.p = nullptr;
        that.n = 0;
        print("mtor", p, n);
        // moved elements are available
    }

    base& operator=(base&& that) noexcept
    {
        std::destroy_n(p, n);
        alloc.deallocate(p, n);

        p = std::exchange(that.p, nullptr);
        n = std::exchange(that.n, 0);

        print("move", p, n);
        return *this;
        // moved elements are available
    }
};

template<typename T>
class vector : private base<T>
{
    std::size_t sz;
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
    const T& operator[](std::size_t i) const;
    T& at(std::size_t);
    const T& at(std::size_t) const;

    std::size_t size() const { return sz; }
    std::size_t capacity() const { return this->n; }

    void reserve(std::size_t);
    void resize(std::size_t, T = T());
    void push_back(const T&);
};

template<typename T>
vector<T>::vector() : base<T>{}
{}

template<typename T>
vector<T>::~vector()
{}

template<typename T>
vector<T>::vector(std::size_t size, T def)
    : base<T>(size), sz{size}
{
    std::fill_n(this->p, size, def);
}

template<typename T>
vector<T>::vector(std::initializer_list<T> list)
    : base<T>(list.size()), sz{list.size()}
{
    std::uninitialized_copy(list.begin(), list.end(), this->p);
}

template<typename T>
vector<T>::vector(const vector<T>& that)
    : base<T>(that.sz), sz(that.sz)
{
    std::uninitialized_copy_n(that.p, that.sz, this->p);
}

template<typename T>
vector<T>& vector<T>::operator=(const vector<T>& that)
{
    if (this == that) return *this;

    if (this->n < that.sz) {
        base<T> new_base{that.sz};
        std::uninitialized_copy_n(that.p, that.sz, new_base);
        std::swap<base<T>>(this, new_base);
    } else {
        std::destroy_n(this->p, this->n);
        std::uninitialized_copy_n(that.p, that.sz, this->p);
    }

    sz = that.sz;
    return *this;
}

template<typename T>
vector<T>::vector(vector<T>&& that)
    : base<T>{}, sz(0)
{
    std::swap<base<T>>(*this, that);
    std::swap(sz, that.sz);
}

template<typename T>
vector<T>& vector<T>::operator=(vector<T>&& that)
{
    if (this == that) return *this;

    std::destroy_n(this->p, this->n);
    this->n = 0;
    std::swap<base<T>>(this, that);

    sz = std::exchange(that.sz, 0);
    return *this;
}

template<typename T>
T& vector<T>::operator[](std::size_t i)
{
    return *(this->p + i);
}

template<typename T>
const T& vector<T>::operator[](std::size_t i) const
{
    return *(this->p + i);
}

template<typename T>
T& vector<T>::at(std::size_t i)
{
    if (i < 0 || sz <= i) throw std::out_of_range();
    return *(this->p + i);
}

template<typename T>
const T& vector<T>::at(std::size_t i) const
{
    if (i < 0 || sz <= i) throw std::out_of_range();
    return *(this->p + i);
}

template<typename T>
void vector<T>::reserve(std::size_t new_n)
{
    if (new_n <= this->n) return;

    // this-n < new_n
    base<T> new_base(new_n);
    new_base = *this;
    std::swap<base<T>>(this, new_base);
}
/*
template<typename T>
void vector<T>::resize(std::size_t, T = T())
{}

template<typename T>
void vector<T>::push_back(const T&)
{}
*/

