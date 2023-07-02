#include <sstream>
#include <algorithm>
#include <ranges>
#include <iterator>
#include <iostream>

int main()
{
  auto floats = std::istringstream("1.1 2.2 3.3\t4.4\n5.5");
  std::ranges::copy(
    std::ranges::istream_view<float>(floats),
    std::ostream_iterator<float>(std::cout, " ")
  );
  std::cout << std::endl;
}
