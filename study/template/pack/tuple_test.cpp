// #define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
// #include <doctest.h>
#include "./tuple.hpp"

constexpr my::tuple<int> one(42);
constexpr my::tuple<int, double> two(42, 53.0);
constexpr my::tuple<int, double, char> three(42, 53.0, 'a');

static_assert(  42 == one.value);
static_assert(  42 == two.value);
static_assert(53.0 == two.rest.value);
static_assert(  42 == three.value);
static_assert(53.0 == three.rest.value);
static_assert( 'a' == three.rest.rest.value);

static_assert(  42 == my::get<0>(one));
static_assert(  42 == my::get<0>(two));
static_assert(53.0 == my::get<1>(two));
static_assert(  42 == my::get<0>(three));
static_assert(53.0 == my::get<1>(three));
static_assert( 'a' == my::get<2>(three));

#include <type_traits>
static_assert(std::is_same_v<int    const&, decltype(my::get<0>(one))>);
static_assert(std::is_same_v<int    const&, decltype(my::get<0>(two))>);
static_assert(std::is_same_v<double const&, decltype(my::get<1>(two))>);
static_assert(std::is_same_v<int    const&, decltype(my::get<0>(three))>);
static_assert(std::is_same_v<double const&, decltype(my::get<1>(three))>);
static_assert(std::is_same_v<char   const&, decltype(my::get<2>(three))>);

int main(){}
