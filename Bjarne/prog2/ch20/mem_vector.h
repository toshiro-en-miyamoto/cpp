#pragma once

#include "mem.h"
#include <initializer_list>

namespace ch20 {

template<typename T, typename A = std::allocator<T>>
struct vector : private mem<T,A>
{
  using allocator_type = mem<T,A>::allocator_type;
  using value_type = mem<T,A>::value_type;
  using size_type = mem<T,A>::size_type;
  using pointer = mem<T,A>::pointer;
  using const_pointer = mem<T,A>::const_pointer;
  using reference = value_type&;
  using const_reference = const value_type&;
  using iterator = pointer;
  using const_iterator = const_pointer;

  explicit vector(size_type capacity, allocator_type = allocator_type());
  vector(std::initializer_list<value_type>, allocator_type = allocator_type());
  ~vector();

  vector(const vector<T,A>&);
  vector<T,A>& operator=(const vector<T,A>&);
  vector(vector<T,A>&&) noexcept;
  vector<T,A>& operator=(vector<T,A>&&) noexcept;

  inline const allocator_type& allocator() const noexcept
  { return this->alloc_; }
  inline const size_type capacity() const noexcept
  { return this->capa_; }
  inline const size_type size() const noexcept
  { return this->size_; }
  inline void set_size(size_type size) noexcept
  { this->size_ = size; }

  inline reference operator[](size_type i) noexcept
  { return *(this->ptr_ + i); }
  inline const_reference operator[](size_type i) const noexcept
  { return *(this->ptr_ + i); }
  inline pointer data() noexcept
  { return this->ptr_; }
  inline const_pointer data() const noexcept
  { return this->ptr_; }

  inline iterator begin() noexcept
  { return this->ptr_; }
  inline const_iterator begin() const noexcept
  { return this->ptr_; }
  inline iterator end() noexcept
  { return this->ptr_ + this->size_; }
  inline const_iterator end() const noexcept
  { return this->ptr_ + this->size_; }

  void reserve(size_type);
  void push_back(const_reference);

  void swap(vector<T,A>&) noexcept;

  template<typename VT, typename AT>
  friend void swap(vector<VT,AT>& ,vector<VT,AT>&) noexcept;
};

template<typename T, typename A>
void vector<T,A>::push_back(const_reference val)
{
  if (0 == capacity()) reserve(8);
  else if (size() == capacity()) reserve(2 * capacity());

  std::uninitialized_fill_n(data() + size(), 1, val);
  set_size(size() + 1);
}

template<typename T, typename A>
void vector<T,A>::reserve(size_type capa)
{
  if (capacity() < capa) {
    vector<T,A> temp(capa);
    std::uninitialized_copy_n(data(), size(), temp.data());
    temp.set_size(size());
    temp.swap(*this);
  }
}

template<typename T, typename A>
vector<T,A>& vector<T,A>::operator=(vector<T,A>&& rhs) noexcept
{
  if (this == &rhs) return *this;

  vector<T,A> temp(std::move(*this));
  rhs.swap(*this);

  return *this;
}

template<typename T, typename A>
vector<T,A>::vector(vector<T,A>&& rhs) noexcept
  : mem<T,A> {std::move(rhs)}
{}

template<typename T, typename A>
vector<T,A>& vector<T,A>::operator=(const vector<T,A>& rhs)
{
  if (this == &rhs) return *this;

  if (capacity() < rhs.capacity()) {
    vector<T,A> temp(rhs.capacity());
    std::uninitialized_copy_n(rhs.data(), rhs.size(), temp.data());
    temp.set_size(rhs.size());
    temp.swap(*this);
  } else {
    std::destroy_n(data(), size());
    std::uninitialized_copy_n(rhs.data(), rhs.size(), data());
    set_size(rhs.size());
  }

  return *this;
}

template<typename T, typename A>
vector<T,A>::vector(const vector<T,A>& rhs)
  : mem<T,A> {rhs.capacity(), rhs.allocator()}
{
  if (0 < capacity()) {
    std::uninitialized_copy_n(rhs.data(), rhs.size(), data());
  }
  set_size(rhs.size());
}

template<typename VT, typename AT>
void swap(vector<VT,AT>& a, vector<VT,AT>& b) noexcept
{
  a.swap(b);
}

template<typename T, typename A>
void vector<T,A>::swap(vector<T,A>& rhs) noexcept
{
  std::swap(this->ptr_, rhs.ptr_);
  std::swap(this->capa_, rhs.capa_);
  std::swap(this->size_, rhs.size_);
}

template<typename T, typename A>
vector<T,A>::vector(std::initializer_list<value_type> list, allocator_type alloc)
  : mem<T,A> {list.size(), alloc}
{
  if (0 < capacity()) {
    std::uninitialized_copy_n(list.begin(), list.size(), data());
  }
  set_size(list.size());
}

template<typename T, typename A>
vector<T,A>::vector(size_type capacity, allocator_type alloc)
  : mem<T,A> {capacity, alloc}
{}

template<typename T, typename A>
vector<T,A>::~vector()
{
  std::destroy_n(data(), size());
}

}
