#include <ch18/vector.h>
#include <algorithm>

my::vector::vector()
  : sz{0}
  , elem{nullptr}
{}

my::vector::vector(std::size_t s)
  : sz{s}
  , elem{new double[s]}
{
    for (int i = 0; i < sz; ++i) {
        elem[i] = 0.0;
    }
}

my::vector::vector(std::initializer_list<double> lst)
  : sz{lst.size()}
  , elem{new double[lst.size()]}
{
    std::copy(lst.begin(), lst.end(), elem);
}

my::vector::~vector()
{
    delete[] elem;
}

my::vector::vector(const my::vector& v)
  : sz{v.sz}
  , elem{new double[v.sz]}
{
    std::copy(v.elem, v.elem + v.sz, elem);
}

my::vector& my::vector::operator=(const my::vector& v)
{
    // to do: check self-assignment
    auto p = new double[v.sz];
    std::copy(v.elem, v.elem + v.sz, p);
    delete[] elem;
    elem = p;
    sz = v.sz;
    return *this;
}

double& my::vector::operator[](std::size_t i)
{
    return elem[i];
}

double my::vector::operator[](std::size_t i) const
{
    return elem[i];
}
