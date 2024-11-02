// g++ -std=c++23 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Wconversion ex13.cpp
/*
  Sieve of Eratosthenes:
  To find all the prime numbers less than or equal to a given integer n,

  1. Create a list of consecutive integers from 2 through n: (2, 3, 4, ..., n).
  2. Initially, let p equal 2, the smallest prime number.
  3. Enumerate the multiples of p by counting in increments of p from 2p to n,
    and mark (false) them in the list
    (these will be 2p, 3p, 4p, ...; the p itself should not be marked).
  4. Find the smallest number in the list greater than p that is not marked (true).
    If there was no such number, stop. Otherwise, let p now equal this new
    number (which is the next prime), and repeat from step 3.
  5. When the algorithm terminates, the numbers remaining not marked (true) in the
    list are all the primes below n.

  The main idea here is that every value given to p will be prime, because if it
  were composite it would be marked as a multiple of some other, smaller prime.
  Note that some of the numbers may be marked more than once
  (e.g., 15 will be marked both for 3 and 5).
*/

// 1. Create a list of consecutive integers from 2 through n: (2, 3, 4, ..., n).
// we start from 0 so as to make the array index identical to the number to check.

/*
  As a refinement, it is sufficient to mark the numbers in step 3 starting from
  p*p, as all the smaller multiples of p will have already been marked at that
  point. This means that the algorithm is allowed to terminate in step 4 when
  p*p is greater than n.

  Another refinement is to initially list odd numbers only, (3, 5, ..., n), and
  count in increments of 2p in step 3, thus marking only odd multiples of p.
  This actually appears in the original algorithm. This can be generalized with
  wheel factorization, forming the initial list only from numbers co-prime with
  the first few primes and not just from odds (i.e., numbers co-prime with 2),
  and counting in the correspondingly adjusted increments so that only such
  multiples of p are generated that are co-prime with those small primes, in the first place.
*/

#include <set>
#include <utility>

[[nodiscard]]
std::set<int>&& composites_by(int number, int sentinel) {
  static std::set<int> composites;
  for (int n = 2 * number; n < sentinel; n += number) {
    composites.insert(n);
  }
  return std::move(composites);
}

#include <vector>

[[nodiscard]]
std::vector<int>&& primes_less_than(int sentinel) {
  std::set<int> composites;
  composites.merge(composites_by(2, sentinel));
  const int half = (sentinel + 1) / 2;
  for (int n = 3; n < half; n += 2) {
    if (!composites.contains(n)) {
      composites.merge(composites_by(n, sentinel));
    }
  }
  static std::vector<int> primes;
  for (int n = 2; n < sentinel; n++) {
    if (!composites.contains(n)) {
      primes.push_back(n);
    }
  }
  return std::move(primes);
}

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../doctest/doctest.h"

TEST_CASE("composites") {
  const std::set<int> expected3{ 4, 6, 8 };
  const auto result3 = composites_by(2, 10);
  CHECK(expected3 == result3);
  const std::set<int> expected4{ 6, 9, 12 };
  const auto result4 = composites_by(3, 15);
  CHECK(expected4 == result4);
}

#include <array>
constexpr std::array test_primes {
  2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47,
  53, 59, 61, 67, 71, 73, 79, 83, 89, 97
};
constexpr int test_sentinel = 100;

TEST_CASE("find primes less than 100") {
  std::vector<int> expected{ test_primes.begin(), test_primes.end() };
  std::vector<int> result{ primes_less_than(100)};
  CHECK(expected == result);
}
