// g++ -std=c++23 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Wconversion bleep.cpp
/*
  Firstly, define a few words that you don't like.
  Then, read in some words using an input stream and print them
  on an output stream, but write out BLEEP instead if a word is
  among the words that you don't like.
*/

#include <array>
#include <vector>
#include <string_view>

using namespace std::literals;

constexpr std::array dislikes{
  "Windows"sv,
  "Microsoft"sv
};

constexpr const std::string_view bleep {"bleep"};

#include <algorithm>

[[nodiscard]] constexpr
const std::string_view& word_or_bleep(const std::string_view& word) {
  namespace rng = std::ranges;
  return rng::find(dislikes, word) == dislikes.end() ? word : bleep;
}

void test() {
  constexpr std::string_view hello = "Hello";
  static_assert(hello == word_or_bleep(hello));

  constexpr std::string_view microsoft = "Microsoft";
  static_assert(bleep == word_or_bleep(microsoft));
}

