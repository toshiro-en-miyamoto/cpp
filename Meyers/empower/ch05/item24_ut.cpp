// g++ item24_ut.cpp -std=c++20
#include <string>
#include <type_traits>

void g()
{
  std::string&& str1 = std::string();
  static_assert(
    std::is_rvalue_reference_v<decltype(str1)>
  );

  auto&& str2 = str1;
  static_assert(
    std::is_lvalue_reference_v<decltype(str2)>
  );

  auto&& str3 = std::move(str1);
  static_assert(
    std::is_rvalue_reference_v<decltype(str3)>
  );
}

int main()
{}
