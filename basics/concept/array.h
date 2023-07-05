#pragma once
#include <iterator>

namespace my {

template <typename T>
class const_iterator
{
public:
  using iterator_category = std::bidirectional_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using value_type = T;
  using pointer = const value_type*;
  using reference = const value_type&;

  const_iterator() = default;
  const_iterator(pointer p) : ptr_{ p } {}

  const_iterator& operator++() noexcept {
    ++ptr_;
    return *this;
  }
  const_iterator operator++(int) noexcept {
    const_iterator temp{ *this };
    ++(*this);
    return temp;
  }

  const_iterator& operator--() noexcept {
    --ptr_;
    return *this;
  }
  const_iterator operator--(int) noexcept {
    const_iterator temp{ *this };
    --(*this);
    return temp;
  }

  reference operator*() const noexcept {
    return *ptr_;
  }
  pointer operator->() const noexcept {
    return ptr_;
  }

  auto operator<=>(const const_iterator& other) const = default;

private:
  pointer ptr_{ nullptr };
};

template<typename T>
class iterator : public const_iterator<T>
{
public:
  using iterator_category = std::bidirectional_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using value_type = T;
  using pointer = value_type*;
  using reference = value_type&;

  // inherit const_iterator constructors
  using const_iterator<T>::const_iterator;

  iterator& operator++() noexcept {
    // call base-class version
    const_iterator<T>::operator++();
    return *this;
  }
  iterator operator++(int) noexcept {
    iterator temp{ *this };
    const_iterator<T>::operator++();
    return temp;
  }

  iterator& operator--() noexcept {
    const_iterator<T>::operator--();
    return *this;
  }
  iterator operator--(int) noexcept {
    iterator temp{ *this };
    const_iterator<T>::operator--();
    return temp;
  }

  reference operator*() const noexcept {
    return const_cast<reference>(const_iterator<T>::operator*());
  }
  pointer operator->() const noexcept {
    return const_cast<pointer>(const_iterator<T>::operator->());
  }
};

template<typename T, std::size_t SIZE>
struct array
{
  using value_type = T;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using pointer = value_type*;
  using const_pointer = const value_type*;
  using reference = value_type&;
  using const_reference = const value_type&;

  using iterator = my::iterator<T>;
  using const_iterator = my::const_iterator<T>;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  constexpr size_type size() const noexcept { return SIZE; }

  iterator begin() {
    return iterator{ &data[0] };
  }
  const_iterator begin() const {
    return const_iterator{ &data[0] };
  }
  const_iterator cbegin() const {
    return begin();
  }
  iterator end() {
    return iterator{ &data[0] + size() };
  }
  const_iterator end() const {
    return const_iterator{ &data[0] + size() };
  }
  const_iterator cend() const {
    return end();
  }

  reverse_iterator rbegin() {
    return reverse_iterator{ end() };
  }
  const_reverse_iterator rbegin() const {
    return const_reverse_iterator{ end() };
  }
  const_reverse_iterator crbegin() const {
    return rbegin();
  }
  reverse_iterator rend() {
    return reverse_iterator{ begin() };
  }
  const_reverse_iterator rend() const {
    return const_reverse_iterator{ begin() };
  }
  const_reverse_iterator crend() const {
    return rend();
  }

  auto operator<=>(const array& t) const noexcept = default;

  T& operator[](size_type index) {
    if (index >= size()) {
      throw std::out_of_range{ "Index out of range" };
    }
    return data[index];
  }
  const T& operator[](size_type index) const {
    if (index >= size()) {
      throw std::out_of_range{ "Index out of range" };
    }
    return data[index];
  }

  // the data is public to make this an aggregate type
  T data[SIZE];
};

template<typename T, std::same_as<T>... Us>
array(T first, Us... rest) -> array<T, 1 + sizeof...(Us)>;

}
