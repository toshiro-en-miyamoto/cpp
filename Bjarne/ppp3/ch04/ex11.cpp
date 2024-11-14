// g++ -std=c++23 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Wconversion -Wno-psabi ex11.cpp
/**
 * \brief Find the largest Fibonacci number that fits in an `int`.
 * 
 * Fibonacci series is the series that starts with 1 1 2 3 5 8 13 21 34.
 * The next number of the series is the sum of the two previous ones.
 * 
 * prev2  prev  fibonacci
 *               1
 *  0      1     1
 *  1      1     2
 *  1      2     3
 * 
 * fibonacci = prev2 + prev  must be less than or equal to <int>::max.
 * 
 * The largest fibonacci number that fits in an `int` can be determined
 * by checking if prev2 > <int>::max - prev.
 */

#include <limits>
#include <stdexcept>

[[nodiscard]]
constexpr int fibonacci(int prev2, int prev)
{
  if (prev2 > std::numeric_limits<int>::max() - prev)
    throw std::runtime_error("** int overflow **");
  else
    return prev2 + prev;
}

#include <iostream>
#include <locale>
#include <string>

struct thousands_sep : std::numpunct<char> {
  char do_thousands_sep()   const { return ','; }  // separate with dot
  std::string do_grouping() const { return "\3"; } // groups of 3 digits
};

int main()
{
  std::cout.imbue(std::locale(std::cout.getloc(), new thousands_sep));

  int fib = 1;
  try {
    int prev2 = 0;
    int prev = fib;
    while (true) {
      std::cout << fib << '\n';
      fib = fibonacci(prev2, prev);
      prev2 = prev;
      prev = fib;
    }
  }
  catch (std::runtime_error& e) {
    std::cout << e.what() << '\n';
  }

  std::cout << '\n';
  std::cout << "max int: " << std::numeric_limits<int>::max() << '\n';
  std::cout << "max fib: " << fib << '\n';
}
