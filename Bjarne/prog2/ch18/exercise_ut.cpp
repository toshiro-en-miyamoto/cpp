// g++ exercise_ut.cpp exercise.cpp -std=c++20
#include "exercise.h"
#include <boost/ut.hpp>
#include <string_view>

int main()
{
  using namespace boost::ut;
  using namespace std::literals;

  "exercise 8"_test = [] {
    const char* s1 = "home";
    const auto r1 = is_palindrome(s1, s1 + strlen(s1) - 1);
    expect(r1 == false);
    const char* s2 = "malayalam";
    const auto r2 = is_palindrome(s2, s2 + strlen(s2) - 1);
    expect(r2 == true);
  };

  "exercise 7"_test = [] {
    const char *s1 = "Niels", *s2 = "Bohr", *sep = "-";

    auto result = cat(s1, s2, sep);
    std::string_view actual_sv{result};
    std::string_view expected{"Niels-Bohr"};
    expect(expected == actual_sv) << actual_sv;
    delete[] result;
  };

  "exercise 6"_test = [] {
    const std::string s1{"Niels"}, s2{"Bohr"}, sep{"-"};
    std::string expected{"Niels-Bohr"};
    std::string actual = cat(s1, s2, sep);
    expect(expected == actual) << actual;
  };

  "exercise 5"_test = [] {
    const std::string s1{"Niels"}, s2{"Bohr"};
    std::string expected{"Niels.Bohr"};
    std::string actual = cat_dot(s1, s2);
    expect(expected == actual) << actual;
  };

  "exercise 3"_test = [] {
    const char  *a1 = "abc",
                *b1 = "abcd";
    expect(strcmp(a1, b1) < 0_i);
    const char  *a2 = "abb",
                *b2 = "abc";
    expect(strcmp(a2, b2) < 0_i);
    const char  *a3 = "abc",
                *b3 = "abc";
    expect(strcmp(a3, b3) == 0_i);
    const char  *a4 = "abd",
                *b4 = "abc";
    expect(strcmp(a4, b4) > 0_i);
    const char  *a5 = "abcd",
                *b5 = "abc";
    expect(strcmp(a5, b5) > 0_i);
  };

  "exercise 2"_test = [] {
    const char  *p = "exercise",
                *x1 = "xe";
    auto found = findx(p, x1);
    expect(found == p+1);
  };

  "exercise 1"_test = [] {
    const char p[] = "exercise";
    auto po = strdup(p);

    std::string_view svp{p};
    std::string_view svpo{po};
    expect(svp == svpo);
    delete[] po;
  };

  "matchx"_test = [] {
    const char  *s1 = "abc",
                *x1 = "abcd";
    expect(matchx(s1, x1) == false);
    const char  *s2 = "abb",
                *x2 = "abc";
    expect(matchx(s2, x2) == false);
    const char  *s3 = "abc",
                *x3 = "abc";
    expect(matchx(s3, x3) == true);
    const char  *s4 = "abd",
                *x4 = "abc";
    expect(matchx(s4, x4) == false);
    const char  *s5 = "abcd",
                *x5 = "abc";
    expect(matchx(s5, x5) == true);
  };

  "strlen"_test = [] {
    const char p[] = "exercise";
    auto len = strlen(p);
    expect(len == 8);
  };
}
