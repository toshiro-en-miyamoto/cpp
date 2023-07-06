// g++ -std=c++20 factorial2.cpp

// a recursive constexpr function
constexpr long long factorial2r(int n)
{
  if (n <= 1) {
    return 1;
  } else {
    return n * factorial2r(n -1);
  }
}

// an iterative constexpr function
constexpr long long factorial2i(int n)
{
  long long factorial{ 1 };

  for (long long i{ 2 }; i <= n; ++i) {
    factorial *= i;
  }

  return factorial;
}

int main()
{
  static_assert(120 == factorial2r(5));
  static_assert(120 == factorial2i(5));
}
