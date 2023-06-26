#pragma once

#include <algorithm>

template<typename T>
class S
{
  T val;
public:
  S(T a);
  const T& get() const;
  T& operator=(const T&);
};

template<typename T>
S<T>::S(T v) : val {v}
{}

template<typename T>
const T& S<T>::get() const
{ return val; }

template<typename T>
T& S<T>::operator=(const T& v)
{
  T new_val {v};
  std::swap(val, new_val);
  return val;
}
