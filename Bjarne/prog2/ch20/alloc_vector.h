#pragma once

#include <memory>
#include <initializer_list>

namespace ch20 {

template<typename T, typename A = std::allocator<T>>
class vector
{
public:
  using allocator_type = A;
  using value_type = T;
  using size_type = std::size_t;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = std::allocator_traits<A>::pointer;
  using const_pointer = std::allocator_traits<A>::const_pointer;
  using iterator = pointer;
  using const_iterator = const_pointer;

  explicit vector(size_type capacity);
  vector(std::initializer_list<value_type>);
  ~vector();

  vector(const vector<T,A>&);
  vector<T,A>& operator=(const vector<T,A>&);
  vector(vector<T,A>&&) noexcept;
  vector<T,A>& operator=(vector<T,A>&&) noexcept;

  inline size_type capacity() const noexcept
  { return capa_; }
  inline size_type size() const noexcept
  { return size_; }

  inline reference operator[](size_type i) noexcept
  { return *(ptr_ + i); }
  inline const_reference operator[](size_type i) const noexcept
  { return *(ptr_ + i); }
  inline pointer data() noexcept
  { return ptr_; };
  inline const_pointer data () const noexcept
  { return ptr_; }

  inline iterator begin()
  { return ptr_; }
  inline const_iterator begin() const
  { return ptr_; }
  inline iterator end()
  { return ptr_ + size_; }
  inline const_iterator end() const
  { return ptr_ + size_; }

  void reserve(size_type);
  void push_back(const_reference);

private:
  void swap(vector<T,A>&) noexcept;

  template<typename VT, typename AT>
  friend void swap(vector<VT,AT>& ,vector<VT,AT>&) noexcept;

  allocator_type alloc;
  pointer ptr_;
  size_type capa_;    // allocated memory
  size_type size_;    // number of elements
};

template<typename T, typename A>
void vector<T,A>::push_back(const_reference val)
{
  if (capa_ == 0) reserve(8);
  else if (size_ == capa_) reserve(2 * capa_);

  std::uninitialized_fill_n(ptr_ + size_, 1, val);
  ++size_;
}

template<typename T, typename A>
void vector<T,A>::reserve(size_type capacity)
{
  if (capa_ < capacity) {
    vector<T,A> lhs(capacity);
    std::uninitialized_copy_n(ptr_, size_, lhs.ptr_);
    lhs.size_ = size_;
    lhs.swap(*this);
  }
}

template<typename T, typename A>
vector<T,A>& vector<T,A>::operator=(vector<T,A>&& rhs) noexcept
{
  if (this == &rhs) return *this;

  //  | temp | this | rhs  |
  vector<T,A> temp(std::move(*this));
  //  | this | null | rhs  |
  rhs.swap(*this);
  //  | this | rhs  | null |

  return *this;
}   // 'this' in temp will be destroyed

template<typename T, typename A>
vector<T,A>::vector(vector<T,A>&& rhs) noexcept
  : alloc {}
  , ptr_  {std::exchange(rhs.ptr_, nullptr)}
  , capa_ {std::exchange(rhs.capa_, 0)}
  , size_ {std::exchange(rhs.size_, 0)}
{}

template<typename T, typename A>
vector<T,A>& vector<T,A>::operator=(const vector<T,A>& rhs)
{
  if (this == &rhs) return *this;

  if (capa_ < rhs.capa_) {
    // need newly allocated memory
    vector<T,A> copy(rhs);
    copy.swap(*this);
  } else {
    // if (rhs.capa_ <= capa_) then use this
    // meaning ptr_ and _capa_ don't change
    std::destroy_n(ptr_, size_);
    std::uninitialized_copy_n(rhs.ptr_, rhs.size_, ptr_);
    size_ = rhs.size_;
  }

  return *this;
}

template<typename T, typename A>
vector<T,A>::vector(const vector<T,A>& rhs)
  : alloc {}, ptr_ {nullptr}
  , capa_ {rhs.capa_}, size_ {rhs.size_}
{
  if (0 < capa_) {
    ptr_ = alloc.allocate(capa_);
    std::uninitialized_copy_n(rhs.ptr_, size_, ptr_);
  }
}

template<typename VT, typename AT>
void swap(vector<VT,AT>& a, vector<VT,AT>& b) noexcept
{
  a.swap(b);
}

template<typename T, typename A>
void vector<T,A>::swap(vector<T,A>& rhs) noexcept
{
  std::swap(ptr_, rhs.ptr_);
  std::swap(capa_, rhs.capa_);
  std::swap(size_, rhs.size_);
}

template<typename T, typename A>
vector<T,A>::vector(std::initializer_list<value_type> list)
  : alloc {}, ptr_ {nullptr}
  , capa_ {list.size()}, size_ {list.size()}
{
  if (0 < capa_) {
    ptr_ = alloc.allocate(capa_);
    std::uninitialized_copy_n(list.begin(), size_, ptr_);
  }
}

template<typename T, typename A>
vector<T,A>::~vector()
{
  if (0 < capa_) {
    std::destroy_n(ptr_, size_);
    alloc.deallocate(ptr_, capa_);
  }
}

template<typename T, typename A>
vector<T,A>::vector(size_type capacity)
  : alloc {}, ptr_ {nullptr}
  , capa_ {capacity}, size_ {0}
{
  if (0 < capa_) ptr_ = alloc.allocate(capa_);
}

}
