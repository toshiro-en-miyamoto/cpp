#pragma once

#include <utility>

#ifndef NDEBUG
#include <iostream>
#endif

struct int_char
{
  int n;
  char c;

  int_char();
  int_char(int, char);
  int_char(const int_char&);
  int_char& operator=(const int_char&);
  int_char(int_char&&);
  int_char& operator=(int_char&&);
  ~int_char();
};

int_char& int_char::operator=(int_char&& rhs)
{
#ifndef NDEBUG
  std::cout << "move " << rhs.n << " over " << n << '\n';
#endif
  n = std::exchange(rhs.n, 0);
  c = std::exchange(rhs.c, 0);
  return *this;
}

int_char::int_char(int_char&& rhs)
  : n {std::exchange(rhs.n, 0)}
  , c {std::exchange(rhs.c, 0)}
{
#ifndef NDEBUG
  std::cout << "mtor " << n << '\n';
#endif
}

int_char& int_char::operator=(const int_char& rhs)
{
#ifndef NDEBUG
  std::cout << "copy " << rhs.n << " over " << n << '\n';
#endif
  n = rhs.n;
  c = rhs.c;
  return *this;
}

int_char::int_char(const int_char& rhs)
  : n {rhs.n}, c {rhs.c}
{
#ifndef NDEBUG
  std::cout << "ctor " << n << '\n';
#endif
}

int_char::int_char()
  : n {0}, c {0}
{
#ifndef NDEBUG
  std::cout << "new  default\n";
#endif
}

int_char::int_char(int num, char ch)
  : n {num}, c {ch}
{
#ifndef NDEBUG
  std::cout << "new  " << n << '\n';
#endif
}

int_char::~int_char()
{
#ifndef NDEBUG
  std::cout << "del  " << n << '\n';
#endif
}
