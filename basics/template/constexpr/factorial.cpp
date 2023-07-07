// g++ -std=c++20 factorial.cpp

// a value metafunction
template<int N>
struct factorial1
{
  static constexpr long long value
  { N * factorial1<N - 1>::value };
};

// factorial1 specialization for 0!
template<>
struct factorial1<0>
{
  static constexpr long long value{ 1 };
};

// a recursive constexpr function
constexpr long long factorial2r(int n)
{
  return n <= 1 ? 1 : n * factorial2r(n -1);
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
  static_assert(120 == factorial1<5>::value);
  static_assert(120 == factorial2r(5));
  static_assert(120 == factorial2i(5));
}
