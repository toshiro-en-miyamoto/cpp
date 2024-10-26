// g++ -c -std=c++23 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Wconversion constant.cpp

// Constant expressions are the expressions that can be evaluated
// at compile time. Such expressions can be used as non-type template
// arguments, array sizes, and in other contexts that require constant
// expressions, e.g.
#include <array>
void constant_array()
{
  std::size_t n = 1;
//  std::array<int, n> a1;  // compile-time error:
  // expression must have a constant value
  // constant.cpp(7, 19): the value of variable "n" (declared at line 6)
  // cannot be used as a constant.
  const std::size_t cn = 2;
  std::array<int, cn> a2; // OK: cn is a constant expression
}

