// g++ x_ut.cpp -std=c++20
#include "x.h"
#include <boost/ut.hpp>
#include <sstream>
#include <vector>

X copy(X a) { return a; }
X copy2(X a) { return X{a.val+1}; }
X& ref_to(X& a) { return a; }
X* make(int i) { X a(i); return new X(a); }
struct XX { X a; X b; };

int main()
{
  using namespace boost::ut;

  "five essential operations"_test = [] {
    std::ostringstream test_out;
    auto cout_buff = std::cout.rdbuf();
    std::cout.rdbuf(test_out.rdbuf());

    {
      std::cout << "local variable\n";
      X loc{4};

      std::cout << "copy construction\n";
      X loc2{loc};

      std::cout << "copy assignment\n";
      loc = X{5};

      std::cout << "copy by value and return\n";
      loc2 = copy(loc);

      std::cout << "calling copy2(loc)\n";
      loc2 = copy2(loc);

      std::cout << "local variable loc3\n";
      X loc3{6};

      std::cout << "call by reference and return\n";
      X& r = ref_to(loc);

      std::cout << "calling make(7) and delete it\n";
      delete make(7);

      std::cout << "calling make(8) and delete it\n";
      delete make(8);

      std::cout << "default values in vector\n";
      std::vector<X> v(4);

      std::cout << "default values in XX\n";
      XX loc4;

      std::cout << "an X on the free store\n";
      X* p = new X{9};
      delete p;

      std::cout << "an array of Xs on the free store\n";
      X* pp = new X[5];
      delete[] pp;
    }

    std::cout.rdbuf(cout_buff);
    expect(
      test_out.str() ==
      "local variable\n"
        "X(int):4(4)\n"

      "copy construction\n"
        "X(X&):4(4)\n"

      "copy assignment\n"
        "X(int):5(5)\n"     // constructing rvalue X{5}
        "X::=():4(5)\n"
        "~X():5(0)\n"       // the rvalue is a temporary

      "copy by value and return\n"
        "X(X&):5(5)\n"      // copying the argument value
        "X(X&):5(5)\n"      // initializing a return value
        "X::=():4(5)\n"     // copy assignment
        "~X():5(0)\n"       // the return value is a temporary
        "~X():5(0)\n"       // the argument is a local variable

      "calling copy2(loc)\n"
        "X(X&):5(5)\n"      // copying the argument value
        "X(int):6(6)\n"     // initializing a return value
        "X::=():5(6)\n"     // copy assignment
        "~X():6(0)\n"       // the return value is a temporary
        "~X():5(0)\n"       // the argument is a local variable

      "local variable loc3\n"
        "X(int):6(6)\n"

      "call by reference and return\n"
        // nothing happens

      "calling make(7) and delete it\n"
        "X(int):7(7)\n"     // local variable in make()
        "X(X&):7(7)\n"      // copy construction in make()
        "~X():7(0)\n"       // destroying X::a in make()
        "~X():7(0)\n"       // delete call ~X()

      "calling make(8) and delete it\n"
        "X(int):8(8)\n"     // local variable in make()
        "X(X&):8(8)\n"      // copy construction in make()
        "~X():8(0)\n"       // destroying X::a in make()
        "~X():8(0)\n"       // delete call ~X()

      "default values in vector\n"
        "X():0(0)\n"        // default constructor
        "X():0(0)\n"        // default constructor
        "X():0(0)\n"        // default constructor
        "X():0(0)\n"        // default constructor

      "default values in XX\n"
        "X():0(0)\n"        // default constructor
        "X():0(0)\n"        // default constructor

      "an X on the free store\n"
        "X(int):9(9)\n"     // instantiate X on the free store
        "~X():9(0)\n"       // deleting X on the free store

      "an array of Xs on the free store\n"
        "X():0(0)\n"        // default constructor
        "X():0(0)\n"        // default constructor
        "X():0(0)\n"        // default constructor
        "X():0(0)\n"        // default constructor
        "X():0(0)\n"        // default constructor
        "~X():0(0)\n"       // deleting the array elements
        "~X():0(0)\n"       // deleting the array elements
        "~X():0(0)\n"       // deleting the array elements
        "~X():0(0)\n"       // deleting the array elements
        "~X():0(0)\n"       // deleting the array elements

      "~X():0(0)\n"         // destroying XX members
      "~X():0(0)\n"         // destroying XX members

      "~X():0(0)\n"         // destroying vector elements
      "~X():0(0)\n"         // destroying vector elements
      "~X():0(0)\n"         // destroying vector elements
      "~X():0(0)\n"         // destroying vector elements

      "~X():6(0)\n"         // loc3
      "~X():6(0)\n"         // loc2
      "~X():5(0)\n"         // loc
    ) << test_out.str();
  };
}
