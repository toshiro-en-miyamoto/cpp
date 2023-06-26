#pragma once
#include <cstddef>
#include <memory>
#include <utility>
#include <initializer_list>
#ifndef NDEBUG
#include <iostream>
#endif

namespace ch19 {

template<typename T>
class mem
{
  std::allocator<T> alloc;
  T* p;             // pointer to memory
  std::size_t n;    // capacity of the memory

  mem(T* p, std::size_t);
public:
  mem();
  ~mem();
  explicit mem(std::size_t);
  mem(const mem&);
  mem& operator=(const mem&);
  mem(mem&&);
  mem& operator=(mem&&);

  T* elem() { return p; }
  const T* elem() const { return p; }

  T& operator[](std::size_t n)
  { return *(p + n); }
  const T& operator[](std::size_t n) const
  { return *(p + n); }

  void reserve(std::size_t);
  std::size_t capacity() const { return n; }
};

template<typename T>
mem<T>::mem(T* p, std::size_t n)
  : alloc {}, p {p}, n {n}
{
#ifndef NDEBUG
  std::cout << "new  " << n << '\n';
#endif
}
 
template<typename T>
mem<T>::mem()
  : alloc {}, p {nullptr}, n {0}
{
#ifndef NDEBUG
  std::cout << "new  0\n";
#endif
}

template<typename T>
mem<T>::~mem()
{
#ifndef NDEBUG
  std::cout << "del  " << n << '\n';
#endif
  alloc.deallocate(p, n);
}

template<typename T>
mem<T>::mem(std::size_t n)
  : alloc {}, p {nullptr}, n {n}
{
#ifndef NDEBUG
  std::cout << "size " << n << '\n';
#endif
  p = alloc.allocate(n);
  // std::uninitialized_fill_n(p, n, T {});
}

template<typename T>
mem<T>::mem(const mem<T>& that)
  : alloc {}, p {nullptr}, n {that.n}
{
#ifndef NDEBUG
  std::cout << "ctor " << n << '\n';
#endif
  p = alloc.allocate(n);
  std::uninitialized_copy_n(that.p, n, p);
}

template<typename T>
mem<T>& mem<T>::operator=(const mem<T>& that)
{
#ifndef NDEBUG
  std::cout << "copy " << that.n
  << ':' << n
  << '\n';
#endif
  if (this == &that) return *this;

  if (n < that.n) {
    mem<T> new_mem { alloc.allocate(that.n), that.n };
    std::uninitialized_copy_n(that.p, that.n, new_mem.p);
    std::destroy_n(p, n);
    std::swap<mem<T>>(new_mem, *this);
  } else {
    std::destroy_n(p, n);
    std::uninitialized_copy_n(that.p, that.n, p);
  }

  return *this;
}

template<typename T>
mem<T>::mem(mem<T>&& that)
  : alloc {}, p {that.p}, n {that.n}
{
#ifndef NDEBUG
  std::cout << "mtor " << n << '\n';
#endif
  that.p = nullptr;
  that.n = 0;
}

template<typename T>
mem<T>& mem<T>::operator=(mem<T>&& that)
{
#ifndef NDEBUG
  std::cout << "move " << that.n << '\n';
#endif
  if (this == &that) return *this;

  std::destroy_n(p, n);
  p = std::exchange(that.p, nullptr);
  n = std::exchange(that.n, 0);

  return *this;
}

template<typename T>
void mem<T>::reserve(std::size_t new_n)
{
#ifndef NDEBUG
  std::cout << "resv " << n
  << ':' << new_n
  << '\n';
#endif
  if (new_n <= n) return;

  mem<T> new_mem {alloc.allocate(new_n), new_n};
  std::uninitialized_copy_n(p, n, new_mem.p);
  std::destroy_n(p, n);
  std::swap<mem<T>>(new_mem, *this);
}

template<typename T>
class vector : protected mem<T>
{
  std::size_t sz;     // the number of Ts
public:
  vector() : sz {0}, mem<T> {} {}
  ~vector() = default;
  explicit vector(std::size_t, T = T {});
  vector(std::initializer_list<T>);

  vector(const vector&) = default;
  vector& operator=(const vector&) = default;
  vector(vector&&) = default;
  vector& operator=(vector&&) = default;

  using mem<T>::operator[];

  std::size_t size() const { return sz; }
  using mem<T>::capacity;

  using mem<T>::reserve;
  void resize(std::size_t, T = T {});
  void push_back(const T&);
};

template<typename T>
vector<T>::vector(std::size_t n, T val)
  : mem<T>(n), sz {n}
{
  std::fill_n(this->elem(), n, val);
}

template<typename T>
vector<T>::vector(std::initializer_list<T> list)
  : mem<T>(list.size()), sz {list.size()}
{
  std::copy(list.begin(), list.end(), this->elem());
}

template<typename T>
void vector<T>::resize(std::size_t new_sz, T val)
{
  reserve(new_sz);
  if (sz < new_sz) {
    std::fill_n(this->elem() + sz, new_sz - sz, val);
  } else if (new_sz < sz) {
    std::destroy(this->elem() + new_sz, this->elem() + sz);
  }
  sz = new_sz;
}

template<typename T>
void vector<T>::push_back(const T& val)
{
  const auto space = capacity();
  if (space == 0) reserve(8);
  else if (sz == space) reserve(2 * space);

  std::fill_n(this->elem() + sz, 1, val);
  ++sz;
}

}   // end of namespace ch19
