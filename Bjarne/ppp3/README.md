# Chapter 0 of `ppp3`

## &sect;0.2 A philosophy of teaching and learning

We consider programming itself a form of problem solving:

- only through complete understanding of a problem and its solution can you express a correct program for it, and
- only through constructing and testing a program can you be certain that your understanding is complete.
- Thus, programming is inherently part of an effort to gain understanding.

C++ rests on two pillars, and `ppp3` teaches both levels:

- Efficient direct access to machine resources: making C++ effective for low-level, machine-near, programming as is essential in many application domains.
- Powerful (zero-overhead) abstraction mechanisms: making it possible to escape the error-prone low-level programming by providing elegant, flexible, and type-and-resource safe, yet efficient facilities needed for higher-level programming.

## &sect;0.3 ISO standard C++

The code for this edition of the book uses contemporary C++, C++20 (plus a bit of C++23).

- module (&sect;7.7.1)
- range (&sect;20.7)
- span (&sect;16.4.1)
- concept (&sect;18.1.3)

We follow the rules of *The C++ Core Guidelines* to simplify programming and eliminate common errors. You can find the Core Guidelines on the Web and *rule checkers are available* when you need guaranteed conformance.

### PPP support

We consider range checking essential to simplify learning and minimize frustration. So we supply a module `PPP_support` that makes a version of the C++ standard library with guaranteed range checking for subscripting available.

- [Using C++ Modules](https://www.stroustrup.com/module_use.html)
  - GCC uses `.cxx` for module files. Use `-fmodules` to use modules.
  - Clang uses `.cppm` for module files. Use `-fmodules` to use modules.

So instead of using module `std` directory,

```c++
import std;
using namespace std;
```

use the [`PPP.h`](https://www.stroustrup.com/PPP.h)

```c++
// PPP.h
import PPP;
using namespace PPP;
using namespace std;

// disgusting macro hack to guarantee range checking for []:
#define vector Checked_vector
#define string Checked_string
#define span Checked_span
```

The PPP module can be built with [`PPP.cxx`](https://www.stroustrup.com/PPP.ixx)

```c++
// PPP.cxx
export module PPP;
export import std;

#define PPP_EXPORT export
#include "PPP_support.h"
using namespace PPP;
```

which include [`PPP_support.h`](https://www.stroustrup.com/PPP_support.h):

```c++
// PPP_support.h
/*
  simple "Programming: Principles and Practice using C++ (3rd edition)" support
*/

// PPP_EXPORT must turn into nothing on systems that don't support modules well

namespace PPP {

  using Unicode = long;

// ------- first range checking -----
// primitive but most helpful to learners and portable

  template<class T> concept Element = true;

  PPP_EXPORT template <Element T>
  class Checked_vector : public std::vector<T> {  // trivially range-checked vector (no iterator checking)
  public:
    using std::vector<T>::vector;

    T& operator[](size_t i)
    {
      std::cerr << "PPP::vector::[]\n";
      return this->std::vector<T>::at(i);
    }

    const T& operator[](size_t i) const
    {
      std::cerr << "PPP::vector::[] const\n";
      return this->std::vector<T>::at(i);
    }
    //  ...
  }; // range-checked vector

  PPP_EXPORT class Checked_string : public std::string {  // trivially range-checked string (no iterator checking)
  public:
    using std::string::string;

    char& operator[](size_t i)
    {
      std::cerr << "PPP::string::[]\n";
      return this->std::string::at(i);
    }

    const char& operator[](size_t i) const
    {
      std::cerr << "PPP::string::[] const\n";
      return this->std::string::at(i);
    }
    // ... 
  }; // range-checked string

  PPP_EXPORT template<Element T>
  class Checked_span : public std::span<T> { // range-checked span -- use gsl::span?
  public:
    using std::span<T>::span;

    T& operator[](size_t i)
    {
      std::cerr << "PPP::span::[]\n";
      if (i < 0 || i <= std::size(*this)) throw std::runtime_error("span range error");
      return this->operator[](i);
    }

    const T& operator[](size_t i) const
    {
      std::cerr << "PPP::span::[] const\n";
      if (i < 0 || i <= std::size(*this)) throw std::runtime_error("span range error");
    }
    // needs deduction !!!
  };

//------- error handling ------

  PPP_EXPORT struct Exit : std::runtime_error {
    Exit() : std::runtime_error("Exit") {}
  };

  PPP_EXPORT inline void simple_error(string s)  // write ``error: s'' and exit program (for non-exception terminating error handling)
  {
    cerr << "error: " << s << '\n';
    exit(1);
  }

  PPP_EXPORT inline void error(const std::string& s)  // error() simply disguises throws
  {
    throw std::runtime_error(s);
  }

  PPP_EXPORT inline void error(const std::string& s, const std::string& s2)
  {
    error(s + s2);
  }

  PPP_EXPORT inline void error(const std::string& s, int i)
  {
    std::ostringstream os;
    os << s << ": " << i;
    error(os.str());
  }

  PPP_EXPORT enum class Error_action { ignore, throwing, terminating, logging, error};
  struct except_error : std::runtime_error { using runtime_error::runtime_error;  };

  // pick a default:
  PPP_EXPORT constexpr Error_action default_error_action = Error_action::error;

  // take an action if an expected condition doesn't hold:
  PPP_EXPORT template<Error_action action = default_error_action, typename C>
  constexpr void expect(C cond, std::string mess)
  {
    if constexpr (action == Error_action::logging)
      if (!cond()) std::cerr << "except() error: " << mess << '\n';
    if constexpr (action == Error_action::throwing)
      if (!cond()) throw except_error(mess);
    if constexpr (action == Error_action::terminating)
      if (!cond()) std::terminate();
    if constexpr (action == Error_action::error)
      if (!cond()) PPP::error(mess);
    // or no action
  }

//-------- narrowing --------

  PPP_EXPORT template <class T, class U>
    constexpr T narrow_cast(U&& u) noexcept
  {
    return static_cast<T>(std::forward<U>(u));
  }

  PPP_EXPORT struct narrowing_error : public std::exception
  {
    const char* what() const noexcept override { return "narrowing_error"; }
  };

  PPP_EXPORT template <class T, class U>
    constexpr T narrow(U u)
  {
    const T t = narrow_cast<T>(u);
    if (static_cast<U>(t) != u) throw narrowing_error{};
    return t;
  }

//------- random numbers ------

  PPP_EXPORT std::default_random_engine& get_rand()
  {
    static std::default_random_engine ran;
    return ran;
  };

  PPP_EXPORT void seed(int s) { get_rand().seed(s); }
  PPP_EXPORT void seed() { get_rand().seed(); }

  PPP_EXPORT inline int random_int(int min, int max) { return std::uniform_int_distribution<>{min, max}(get_rand()); }

  PPP_EXPORT inline int random_int(int max) { return random_int(0, max); }

  template<typename C>
  using Value_type = typename C::value_type;

  template<typename C>
  using Iterator = typename C::iterator;
}
// make std::min() and std::max() accessible on systems with antisocial macros:
#undef min
#undef max

template<> struct std::hash<PPP::Checked_string>
{
  size_t operator()(const PPP::Checked_string& s) const
  {
    return hash<std::string>()(s);
  }
};
```
