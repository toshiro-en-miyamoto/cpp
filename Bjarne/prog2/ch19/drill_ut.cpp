// g++ drill_ut.cpp -std=c++20
#include "drill.h"
#include <boost/ut.hpp>
#include <string>
#include <vector>

int main()
{
  using namespace boost::ut;
  using namespace boost::ut::bdd;
  using namespace boost::ut::literals;

  "ch19 drill"_test = [] {
    given("S<int>") = [] {
      S<int> s {1};
      when("access val") = [&] {
        expect(s.get() == 1_i);
      };
      when("assign val") = [&] {
        s = 2;
        expect(s.get() == 2_i);
      };
    };
    given("S<char>") = [] {
      S<char> s {'a'};
      when("access val") = [&] {
        expect(s.get() == 'a');
      };
      when("assign val") = [&] {
        s = 'b';
        expect(s.get() == 'b');
      };
    };
    given("S<double>") = [] {
      S<double> s {1.0};
      when("access val") = [&] {
        expect(s.get() == 1.0_d);
      };
      when("assign val") = [&] {
        s = 2.0;
        expect(s.get() == 2.0_d);
      };
    };
    given("S<std::string>") = [] {
      S<std::string> s {"str"};
      when("access val") = [&] {
        expect(s.get() == "str");
      };
      when("assign val") = [&] {
        s = "xyz";
        expect(s.get() == "xyz");
      };
    };
    given("S<std::vector<int>>") = [] {
      S<std::vector<int>> s {{1, 2}};
      when("access val") = [&] {
        expect(s.get()[0] == 1_i);
        expect(s.get()[1] == 2_i);
      };
      when("assign val") = [&] {
        s = S<std::vector<int>>{{3, 4}};
        expect(s.get()[0] == 3_i);
        expect(s.get()[1] == 4_i);
      };
    };
  };
}
