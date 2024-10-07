/* g++ -std=c++20 cin_sensitive_to_type.cpp
This test code is to understand the sensitivity of std::istream.
According to the section 2.3 of PPP3,
  - The input operation >> ("get from") is sensitive to type.
  - Reading of strings is terminated by a whitespace.
*/

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../doctest/doctest.h"
#include <iostream>
#include <sstream>
#include <utility>

[[nodiscard]]
std::pair<std::string, int> read_string_int(std::istream& is)
{
  std::string str ="???";
  int num = -1;
  is >> str >> num;
  return {str, num};
}

TEST_CASE("the input operation is sensitive to type")
{
  std::istringstream buf1{"Carlos 22"};
  auto [name, age] = read_string_int(buf1);
  CHECK(name == "Carlos");
  CHECK(age == 22);
}

TEST_CASE("reading of string is terminated by a whitespace")
{
  std::istringstream buf1{"Hello Carlos 22"};
  auto [name, age] = read_string_int(buf1);
  CHECK(name == "Hello");
  CHECK(age == 0);    // reading "Carlos" as int results in 0
}
