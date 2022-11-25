#include <cstddef>
#include <memory>
#include <utility>
#include <string_view>
#include <iostream>

struct M
{
    std::allocator<char> alloc;
    char* p;
    std::size_t n;

    M() : alloc{}, p{nullptr}, n{0}
    {}
    explicit M(std::size_t size)
        : alloc{}, p{nullptr}, n{size}
    {
        p = alloc.allocate(n);
        // someone has to initialize n elements in p
    }
    ~M()
    {
        std::destroy_n(p, n);
        alloc.deallocate(p, n);
    }
    M(const M& that)
        : alloc{that.alloc}
        , p{alloc.allocate(that.n)}
        , n{that.n}
    {
        // someone has to initialize n elements in p
    }
    M& operator=(const M& that)
    {
        if (this == &that) return *this;
        p = alloc.allocate(that.n);
        n = that.n;
        return *this;
        // someone has to initialize n elements in p
    }
    M(M&& that)
        : alloc{that.alloc}
        , p{that.p}
        , n{that.n}
    {
        that.p = nullptr;
        that.n = 0;
        // elements in p are initialized already
    }
    M& operator=(M&& that) noexcept
    {
        if (this == &that) return *this;

        std::destroy_n(p, n);
        alloc.deallocate(p, n);

        p = std::exchange(that.p, nullptr);
        n = std::exchange(that.n, 0);
        return *this;
    }
};

class A : private M
{
public:
    A() : M()
    {}
    explicit A(std::size_t n) : M{n}
    {}
    A(std::string_view s) : M(s.size())
    {
        std::uninitialized_copy(s.begin(), s.end(), this->p);
    }
    ~A() {}
    char operator[](std::size_t i) const
    {
        return *(this->p + i);
    }
};

int main()
{
    A a1{"abc"};
    for (auto i = 0; i < 3; ++i) {
        std::cout << a1[i];
    }
    std::cout << '\n';
}
