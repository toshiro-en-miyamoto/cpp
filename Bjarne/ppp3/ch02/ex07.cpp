// g++ -std=c++23 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Wconversion ex07.cpp
/*
  req & test cases: 19:30 - 20:00
  design, coding and test: 20:00 - 20:45

  Sort three words. If we have Steinbeck, Fitzgerald, Hemingway,
  the result should be Fitzgerald, Hemingway, Steinbeck.

  Sort algorithm: selection sort
*/

// test cases
// (01) input {"Steinbeck", "Fitzgerald", "Hemingway"}
//      expected {"Fitzgerald", "Hemingway", "Steinbeck"}
// (02) input {"stringstream", "streambuf", "string"}
//      expected {"streambuf", "string", "stringstream"}

#include <utility>
#include <array>
#include <string_view>

using Elements = std::array<std::string_view, 3>;

constexpr
std::pair<Elements, Elements> tc[] {
  {
    {"Steinbeck", "Fitzgerald", "Hemingway"},
    {"Fitzgerald", "Hemingway", "Steinbeck"}
  }, {
    {"stringstream", "streambuf", "string"},
    {"streambuf", "string", "stringstream"}
  }
};

#include <algorithm>

[[nodiscard]] constexpr
Elements sort(Elements input)
{
  for(auto it_span{input.begin()}; it_span != input.end(); it_span++) {
    std::swap(*it_span, *std::min_element(it_span, input.end()));
  }
  return input;
}

int main()
{
  static_assert(sort(tc[0].first) == tc[0].second);
  static_assert(sort(tc[1].first) == tc[1].second);

  return 0;
}
