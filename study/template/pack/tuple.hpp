#include <cstddef>

namespace my {

// the primary template
template <typename T, typename... Ts>
struct tuple
{
  constexpr tuple(const T& t, const Ts&... ts)
  : value{t}, rest{ts...} {}

  constexpr std::size_t size() const
  { return 1 + rest.size(); }

  T value;
  tuple<Ts...> rest;
};

// the partial specialization
template <typename T>
struct tuple<T>
{
  constexpr tuple(const T& t)
  : value{t} {}

  constexpr std::size_t size() const
  { return 1; }

  T value;
};

// getter helpers
#include <cstddef>

// the type of the element at the N index in the tuple
template <std::size_t N, typename T, typename... Ts>
struct nth_type : nth_type<N-1, Ts...>
{
  static_assert(N < sizeof...(Ts) + 1);
};

template <typename T, typename... Ts>
struct nth_type<0, T, Ts...>
{
  using value_type = T;
};

// class template for retrieving the value
template <std::size_t N>
struct getter
{
  template <typename... Ts>
  constexpr static const typename nth_type<N, Ts...>::value_type&
  get(const tuple<Ts...>& t) noexcept
  {
    return getter<N-1>::get(t.rest);
  }
};

template <>
struct getter<0>
{
  template <typename T, typename... Ts>
  constexpr static const T&
  get(const tuple<T, Ts...>& t) noexcept
  {
    return t.value;
  }
};

// function template that takes a tuple as an argument and
// returns a reference to the element at the N index in the tuple
template <std::size_t N, typename... Ts>
constexpr const typename nth_type<N, Ts...>::value_type&
get(const tuple<Ts...>& t) noexcept
{
  return getter<N>::get(t);
}

}; // namespace my
