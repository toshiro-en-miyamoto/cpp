// g++ -std=c++20 factorial1.cpp

// a value metafunction
template<int N>
struct factorial1
{
  static constexpr long long value {
    N * factorial1<N - 1>::value
  };
};

// factorial1 specialization for 0!
template<>
struct factorial1<0>
{
  static constexpr long long value{ 1 };
};

int main()
{
  static_assert(120 == factorial1<5>::value);
}
