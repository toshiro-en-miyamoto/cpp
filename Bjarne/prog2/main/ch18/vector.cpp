#include <ch18/vector.h>
#include <algorithm>

ch18::vector::vector()
    : sz{0}
    , elem{nullptr}
{}

ch18::vector::vector(std::size_t s)
    : sz{s}
    , elem{new double[s]}
{
    for (int i = 0; i < sz; ++i) {
        elem[i] = 0.0;
    }
}

ch18::vector::vector(std::initializer_list<double> lst)
    : sz{lst.size()}
    , elem{new double[lst.size()]}
{
    std::copy(lst.begin(), lst.end(), elem);
}

ch18::vector::~vector()
{
    delete[] elem;
}

ch18::vector::vector(const ch18::vector& v)
    : sz{v.sz}
    , elem{new double[v.sz]}
{
    std::copy(v.elem, v.elem + v.sz, elem);
}

ch18::vector& ch18::vector::operator=(const ch18::vector& v)
{
    // to do: check self-assignment
    auto p = new double[v.sz];
    std::copy(v.elem, v.elem + v.sz, p);
    delete[] elem;
    elem = p;
    sz = v.sz;
    return *this;
}

ch18::vector::vector(ch18::vector&& v)
    : sz{v.sz}
    , elem{v.elem}
{
    v.sz = 0;           // make v the empty vector
    v.elem = nullptr;
}

ch18::vector& ch18::vector::operator=(vector&& v)
{
    delete[] elem;      // throw away old elements
    elem = v.elem;      // steal v's elements
    sz = v.sz;
    v.elem = nullptr;   // mak v the empty vector
    v.sz = 0;
    return *this;
}

double& ch18::vector::operator[](std::size_t i)
{
    return elem[i];
}

double ch18::vector::operator[](std::size_t i) const
{
    return elem[i];
}
