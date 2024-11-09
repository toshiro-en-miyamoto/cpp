// g++ -std=c++23 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Wconversion -Wno-psabi ex17.cpp
/*!
 * \brief Solves quadratic equations.
 * 
 * A quadratic equation in `x` is an equation that can be written in
 * the standard form; `* a*x2 + b*x + c = 0`, where `a`, `b` and `c`
 * are real numbers and `a != 0`.
 * Since quadratics have a degree equal to two, therefore there will
 * be two solutions for the equation,
 * `x = [ -b +/- sqrt( b2 - 4*a*c ) ] / (2*a)`
 * 
 * Domain errors include:
 *  - `a` is 0
 *  - `(b*b - 4*a*c) is less then 0
 * 
 * \param a The coefficient for `x2`. Refer to the domain errors above.
 * \param b The coefficient for `x1`. `b` may be 0.
 * \param c The coefficient for `x0`. `c` may be 0.
 * \return A pair of real values as the solution. The values may be identical.
 *    If a domain error occurs, `std::nullopt` is returned.
 */

#include <optional>
#include <utility>
#include <cmath>
#include <stdexcept>

template <std::floating_point T>
[[nodiscard]]
std::optional<std::pair<T, T>> sol_quad(T a, T b, T c) noexcept
{
  const T zero = static_cast<T>(0.0);
  if (zero == a) return std::nullopt;

  const T root = b*b - 4*a*c;
  if (zero > root) {
    return std::nullopt;
  } else if (zero == root) {
    const T ans = -b / (2*a);
    return std::make_pair(ans, ans);
  } else {
    const T v = sqrt(root);
    const T ans1 = (-b + v) / (2*a);
    const T ans2 = (-b - v) / (2*a);
    return ans1 < ans2
      ? std::make_pair(ans1, ans2)
      : std::make_pair(ans2, ans1);
  }
}

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#define DOCTEST_CONFIG_VOID_CAST_EXPRESSIONS    // for [[nodiscard]]
#include "../doctest/doctest.h"

TEST_CASE("solve quadratic equations")
{
  REQUIRE_FALSE(sol_quad(0.0, 1.0, 2.0));
  REQUIRE_FALSE(sol_quad(1.0, 1.0, 4.0));
  CHECK_EQ(sol_quad(1.0, -2.0, 1.0), std::make_pair(1.0, 1.0));
  CHECK_EQ(sol_quad(1.0, 0.0, -1.0), std::make_pair(-1.0, 1.0));
}
