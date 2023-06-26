#include "vector.h"
#include <algorithm>

vector::vector()
  : sz{0}
  , elem{nullptr}
{}

vector::vector(std::size_t s)
  : sz{s}
  , elem{new double[s]}
{
  for (int i = 0; i < sz; ++i) {
    elem[i] = 0.0;
  }
}

vector::vector(std::initializer_list<double> lst)
  : sz{lst.size()}
  , elem{new double[lst.size()]}
{
  std::copy(lst.begin(), lst.end(), elem);
}

vector::~vector()
{
  delete[] elem;
}

vector::vector(const vector& v)
  : sz{v.sz}
  , elem{new double[v.sz]}
{
  std::copy(v.elem, v.elem + v.sz, elem);
}

vector& vector::operator=(const vector& v)
{
  // to do: check self-assignment
  auto p = new double[v.sz];
  std::copy(v.elem, v.elem + v.sz, p);
  delete[] elem;
  elem = p;
  sz = v.sz;
  return *this;
}

vector::vector(vector&& v)
  : sz{v.sz}
  , elem{v.elem}
{
  v.sz = 0;           // make v the empty vector
  v.elem = nullptr;
}

vector& vector::operator=(vector&& v)
{
  delete[] elem;      // throw away old elements
  elem = v.elem;      // steal v's elements
  sz = v.sz;
  v.elem = nullptr;   // mak v the empty vector
  v.sz = 0;
  return *this;
}

double& vector::operator[](std::size_t i)
{
  return elem[i];
}

double vector::operator[](std::size_t i) const
{
  return elem[i];
}
