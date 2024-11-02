// g++ -std=c++23 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Wconversion ex12.cpp
/*
  Find all the prime numbers between 1 and 100.
  - a function that will check if a number is prime
    (i.e., see if the number can be divided by a prime number smaller than itself)
    using a vector of primes in order (primes[0]==2, primes[1]==3, primes[2]==5).
  - a loop that goes from 1 to 100,
    check each number to see if it is a prime, and
    store each prime found in a vector
  - check the result by comparing the prime found with the vector primes.
*/

#include <array>

constexpr std::array primes {
  2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47,
  53, 59, 61, 67, 71, 73, 79, 83, 89, 97
};

constexpr auto size_primes = primes.size();

/*
    n   p[]   prime?
     2   n/a  true
     3   2    true
     4   2    false
     5   2    -
     5   3    -
     5   4    true
     6   2    false
     7   2    -
     7   3    -
     7   5    true
     8   2    false
     9   2    -
     9   3    false
    10   2    false
    11   2    -
    11   3    -
    11   5    -
    11   7    true   
*/
[[nodiscard]]
constexpr bool is_prime(int n) {
  for (int i = 0; primes[i] < n; i++) {
    // return true if n can be divided by primes[i]
    if (n % primes[i] == 0)
      return false;
    else
      continue;
  }
  // else return false
  return true;
}

static_assert(is_prime(2));
static_assert(is_prime(3));
static_assert(!is_prime(4));
static_assert(is_prime(5));
static_assert(!is_prime(6));
static_assert(is_prime(7));
static_assert(!is_prime(8));
static_assert(!is_prime(9));
static_assert(!is_prime(10));
static_assert(is_prime(11));
static_assert(!is_prime(12));
static_assert(is_prime(13));

#include <vector>
#include <ranges>
#include <utility>

[[nodiscard]]
std::vector<int>&& find_primes_with(std::vector<int>&& primes_found) {
  for (int n : std::views::iota(2, 100)
      | std::views::filter(is_prime)
  ) {
    primes_found.push_back(n);
  }
  return std::move(primes_found);
}

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../doctest/doctest.h"

TEST_CASE("find prime") {
  std::vector<int> empty;
  auto result{ find_primes_with(std::move(empty)) };
  std::vector expected( primes.begin(), primes.end() );
  CHECK(result == expected);
}
