// g++ item02_ut.cpp -std=c++20
#include <boost/ut.hpp>
#include <type_traits>
#include <initializer_list>
#include <typeinfo>

// for auto detection test
void func(int, char) {};

// for braced-init-list expression test
template<typename T>
void f(T arg)
{};

template<typename T>
const std::type_info& f_list(std::initializer_list<T> arg)
{ return typeid(T); };

int main()
{
  using namespace boost::ut;
  using namespace boost::ut::bdd;

  "auto deduction"_test = [] {
    given("auto T") = [] {
      when("initialized") = [] {
        auto x = 27;
        expect(std::is_same<decltype(x), int>::value);
      };
    };
    given("const auto T") = [] {
      when("initialized") = [] {
        int x = 27;
        const auto cx = x;
        expect(std::is_same<decltype(cx), const int>::value);
      };
    };
    given("const auto& T") = [] {
      when("initialized") = [] {
        int x = 27;
        const auto& rx = x;
        expect(std::is_same<decltype(rx), const int&>::value);
      };
    };
    given("auto&& T") = [] {
      when("initialized with non-const") = [] {
        int x = 27;
        auto&& refref1 = x;
        expect(std::is_same<decltype(refref1), int&>::value);
      };
      when("initialized with const") = [] {
        const int cx = 27;
        auto&& refref2 = cx;
        expect(std::is_same<decltype(refref2), const int&>::value);
      };
      when("initialized with literal") = [] {
        auto&& refref3 = 27;
        expect(std::is_same<decltype(refref3), int&&>::value);
      };
    };
    given("const char[]") = [] {
      when("auto T is initialized") = [] {
        const char name[] = "Briggs";
        auto arr1 = name;
        expect(std::is_same<decltype(arr1), const char*>::value);
      };
      when("auto T& is initialized") = [] {
        const char name[] = "Briggs";
        auto& arr2 = name;
        expect(std::is_same<decltype(arr2), const char (&)[7]>::value);
      };
    };
    given("void func(int, char)") = [] {
      when("auto T is initialized") = [] {
        auto func1 = func;
        expect(std::is_same<decltype(func1), void (*)(int, char)>::value);
      };
      when("auto T& is initialized") = [] {
        auto& func2 = func;
        expect(std::is_same<decltype(func2), void (&)(int, char)>::value);
      };
    };
  };

  "initialization of auto"_test = [] {
    given("C++98 syntaxes") = [] {
      when("copy initialization") = [] {
        auto x = 27;
        expect(std::is_same<decltype(x), int>::value);
      };
      when("direct initialization") = [] {
        auto x(27);
        expect(std::is_same<decltype(x), int>::value);
      };
    };
    given("C++11 syntaxes") = [] {
      when("copy list-initialization") = [] {
        auto x = {27};
        expect(std::is_same<decltype(x), std::initializer_list<int>>::value);
      };
      when("direct list-initialization") = [] {
        auto x {27};
        expect(std::is_same<decltype(x), int>::value);
      };
    };
  };

  "braced-init-list expression"_test = [] {
    given("a template function taking T") = [] {
      // does not compile
      // f({11, 23, 9});
    };
    given("a template function taking list") = [] {
      expect(f_list({11, 23, 9}) == typeid(int));
    };
  };
}
