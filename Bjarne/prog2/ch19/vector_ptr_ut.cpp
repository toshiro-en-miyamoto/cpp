// g++ vector_ptr_ut.cpp -std=c++20
#include "vector_ptr.h"
#include <boost/ut.hpp>
#include <my/hijack.h>

struct X
{
  int val;
  X();
  X(int);
  ~X();
  X(const X&);
  X& operator=(const X&);
};

X& X::operator=(const X& that)
{
#ifndef NDEBUG
  std::cout << "X::copy(" << that.val << ")\n";
#endif
  val = that.val;
  return *this;
}

X::X(const X& that)
  : val {that.val}
{
#ifndef NDEBUG
  std::cout << "X::ctor(" << val << ")\n";
#endif
}

X::X()
  : val {0}
{
#ifndef NDEBUG
  std::cout << "X()\n";
#endif
}

X::X(int v)
  : val {v}
{
#ifndef NDEBUG
  std::cout << "X(" << val << ")\n";
#endif
}

X::~X()
{
#ifndef NDEBUG
  std::cout << "~X(" << val << ")\n";
#endif
}

int main()
{
  using namespace boost::ut;
  using namespace boost::ut::bdd;

  "vector ptr"_test = [] {
    given("default constructor") = [] {
      my::hijack hj {std::cout};
      when("instantiated") = [] {
        ch19::vector<int> v;
        expect(v.capacity() == 0_ul);
        expect(v.size() == 0_ul);
      };
      const auto actual = hj.release();
      expect(actual ==
        "new  0\n"
        "del  0\n"
      ) << actual;
    };
    given("size constructor") = [] {
      my::hijack hj1 {std::cout};
      when("instantiated with implicit default") = [] {
        ch19::vector<int> v(2);
        expect(v.capacity() == 2_ul);
        expect(v.size() == 2_ul);

        int sum = 0;
        for (auto i = 0; i < v.size(); ++i) {
          sum += v[i];
        }
        expect(sum == 0_i);
      };
      const auto actual1 = hj1.release();
      expect(actual1 ==
        "size 2\n"
        "del  2\n"
      ) << actual1;
      my::hijack hj2 {std::cout};
      when("instantiated with implicit default") = [] {
        ch19::vector<int> v(3, 4);
        expect(v.capacity() == 3_ul);
        expect(v.size() == 3_ul);

        int sum = 0;
        for (auto i = 0; i < v.size(); ++i) {
          sum += v[i];
        }
        expect(sum == 12_i);
      };
      const auto actual2 = hj2.release();
      expect(actual2 ==
        "size 3\n"
        "del  3\n"
      ) << actual2;
    };
    given("ctor taking initializer") = [] {
      my::hijack hj {std::cout};
      when("instantiated with list") = [] {
        ch19::vector<int> v {1, 2, 3};
        expect(v.capacity() == 3_i);
        expect(v.size() == 3_i);

        int sum = 0;
        for (auto i = 0; i < v.size(); ++i) {
          sum += v[i];
        }
        expect(sum == 6_i);
      };
      const auto actual = hj.release();
      expect(actual ==
        "list 3\n"
        "del  3\n"
      ) << actual;
    };
    given("copy ctor") = [] {
      my::hijack hj {std::cout};
      when("instantiated with three-elem vector") = [] {
        const ch19::vector<int> v1 {1, 2, 3};
        ch19::vector<int> v {v1};
        expect(v.capacity() == 3_i);
        expect(v.size() == 3_i);

        int sum = 0;
        for (auto i = 0; i < v.size(); ++i) {
          sum += v[i];
        }
        expect(sum == 6_i);
      };
      const auto actual = hj.release();
      expect(actual ==
        "list 3\n"
        "ctor 3\n"
        "del  3\n"
        "del  3\n"
      ) << actual;
    };
    given("copy assignment operator") = [] {
      my::hijack hj {std::cout};
      when("three-elem vector assigned to empty") = [] {
        const ch19::vector<int> v1 {1, 2, 3};
        ch19::vector<int> v {};
        v = v1;
        expect(v.capacity() == 3_i);
        expect(v.size() == 3_i);

        int sum = 0;
        for (auto i = 0; i < v.size(); ++i) {
          sum += v[i];
        }
        expect(sum == 6_i);
      };
      const auto actual = hj.release();
      expect(actual ==
        "list 3\n"
        "new  0\n"
        "copy 3\n"
        "del  3\n"
        "del  3\n"
      ) << actual;
    };
    given("move ctor") = [] {
      my::hijack hj {std::cout};
      when("instantiated with three-elem vector") = [] {
        ch19::vector<int> v1 {1, 2, 3};
        ch19::vector<int> v {std::move(v1)};
        expect(v.capacity() == 3_i);
        expect(v.size() == 3_i);

        int sum = 0;
        for (auto i = 0; i < v.size(); ++i) {
          sum += v[i];
        }
        expect(sum == 6_i);
      };
      const auto actual = hj.release();
      expect(actual ==
        "list 3\n"
        "mtor 3\n"
        "del  3\n"
        "del  0\n"
      ) << actual;
    };
    given("move assignment operator") = [] {
      my::hijack hj {std::cout};
      when("three-elem vector assigned to empty") = [] {
        ch19::vector<int> v1 {1, 2, 3};
        ch19::vector<int> v {};
        v = std::move(v1);
        expect(v.capacity() == 3_i);
        expect(v.size() == 3_i);

        int sum = 0;
        for (auto i = 0; i < v.size(); ++i) {
          sum += v[i];
        }
        expect(sum == 6_i);
      };
      const auto actual = hj.release();
      expect(actual ==
        "list 3\n"
        "new  0\n"
        "move 3\n"
        "del  3\n"
        "del  0\n"
      ) << actual;
    };
    given("reserve()") = [] {
      my::hijack hj {std::cout};
      when("one-slot vector reserves three") = [] {
        ch19::vector<int> v {1};
        v.reserve(3);
        expect(v.capacity() == 3_ul);
        expect(v.size() == 1_ul);
      };
      const auto actual = hj.release();
      expect(actual ==
        "list 1\n"
        "resv 3\n"
        "del  3\n"
      ) << actual;
    };
    given("resize() vector<int>") = [] {
      my::hijack hj1 {std::cout};
      when("one-slot resizes three defaults") = [] {
        ch19::vector<int> v {1};
        v.resize(3);
        expect(v.capacity() == 3_ul);
        expect(v.size() == 3_ul);

        int sum = 0;
        for (auto i = 0; i < v.size(); ++i)
          sum += v[i];
        expect(sum == 1_i) << sum;
      };
      const auto actual1 = hj1.release();
      expect(actual1 ==
        "list 1\n"
        "resv 3\n"
        "del  3\n"
      ) << actual1;

      my::hijack hj2 {std::cout};
      when("one-slot resizes three 2s") = [] {
        ch19::vector<int> v {1};
        v.resize(3, 2);
        expect(v.capacity() == 3_ul);
        expect(v.size() == 3_ul);

        int sum = 0;
        for (auto i = 0; i < v.size(); ++i)
          sum += v[i];
        expect(sum == 5_i) << sum;
      };
      const auto actual2 = hj2.release();
      expect(actual2 ==
        "list 1\n"
        "resv 3\n"
        "del  3\n"
      ) << actual2;

      my::hijack hj3 {std::cout};
      when("three-slot resizes one") = [] {
        ch19::vector<int> v {1, 2, 3};
        v.resize(1);
        expect(v.capacity() == 3_ul);
        expect(v.size() == 1_ul);

        int sum = 0;
        for (auto i = 0; i < v.size(); ++i)
          sum += v[i];
        expect(sum == 1_i) << sum;
      };
      const auto actual3 = hj3.release();
      expect(actual3 ==
        "list 3\n"
        "resv 1\n"
        "del  3\n"
      ) << actual3;
    };
    given("push_back()") = [] {
      my::hijack hj1 {std::cout};
      when("pushed back to empty") = [] {
        ch19::vector<int> v;
        v.push_back(1);
        expect(v.capacity() == 8_ul);
        expect(v.size() == 1_ul);

        int sum = 0;
        for (auto i = 0; i < v.size(); ++i)
          sum += v[i];
        expect(sum == 1_i) << sum;
      };
      const auto actual1 = hj1.release();
      expect(actual1 ==
        "new  0\n"
        "resv 8\n"
        "del  8\n"
      ) << actual1;

      my::hijack hj2 {std::cout};
      when("pushed back to eight-slot") = [] {
        ch19::vector<int> v {1, 2, 3, 4, 5, 6, 7, 8};
        v.push_back(9);
        expect(v.capacity() == 16_ul);
        expect(v.size() == 9_ul);

        int sum = 0;
        for (auto i = 0; i < v.size(); ++i)
          sum += v[i];
        expect(sum == 45_i) << sum;
      };
      const auto actual2 = hj2.release();
      expect(actual2 ==
        "list 8\n"
        "resv 16\n"
        "del  16\n"
      ) << actual2;
    };
    given("resize() vector<X>") = [] {
      my::hijack hj1 {std::cout};
      when("two-slot resizes three defaults") = [] {
        ch19::vector<X> v {X{1}, X{2}};
        v.resize(3);
        expect(v.capacity() == 3_ul);
        expect(v.size() == 3_ul);

        int sum = 0;
        for (auto i = 0; i < v.size(); ++i)
          sum += v[i].val;
        expect(sum == 3_i) << sum;
      };
      const auto actual1 = hj1.release();
      expect(actual1 ==
        // ch19::vector<X> v {X{1}, X{2}};
        "X(1)\n"
        "X(2)\n"
                        // calling ctor taking init list
        "X()\n"         // make_unique<X[]>(list.size(): 2)
        "X()\n"
        "list 2\n"
        "X::copy(1)\n"  // copy(list.begin(), list.end(), ptr.get());
        "X::copy(2)\n"

        "~X(2)\n"
        "~X(1)\n"
        // v.resize(3);
        "X()\n"         // default val = X()
        "resv 3\n"      // v.reserve(3)
                        // if (space < new_space) {
        "X()\n"         //   make_unique<X[]>(new_space: 3)
        "X()\n"
        "X()\n"
        "X::copy(1)\n"  //   copy_n(ptr.get(), 2, new_ptr.get());
        "X::copy(2)\n"
                        //   swap(ptr, new_ptr);
        "~X(2)\n"       // }
        "~X(1)\n"
                        // v.resize continues
                        // if (sz < new_sz)
        "X::copy(0)\n"  //   fill_n(ptr.get()+2, 3-2, val);
        "~X(0)\n"       // deleting default val

        "del  3\n"
        "~X(0)\n"
        "~X(2)\n"
        "~X(1)\n"
      ) << actual1;

      my::hijack hj2 {std::cout};
      when("one-slot resizes three 2s") = [] {
        ch19::vector<X> v {X{1}};
        v.resize(3, X{2});
        expect(v.capacity() == 3_ul);
        expect(v.size() == 3_ul);

        int sum = 0;
        for (auto i = 0; i < v.size(); ++i)
          sum += v[i].val;
        expect(sum == 5_i) << sum;
      };
      const auto actual2 = hj2.release();
      expect(actual2 ==
        "X(1)\n"
        "X()\n"
        "list 1\n"
        "X::copy(1)\n"
        "~X(1)\n"

        "X(2)\n"
        "resv 3\n"
        "X()\n"
        "X()\n"
        "X()\n"
        "X::copy(1)\n"
        "~X(1)\n"
        "X::copy(2)\n"
        "X::copy(2)\n"
        "~X(2)\n"

        "del  3\n"
        "~X(2)\n"
        "~X(2)\n"
        "~X(1)\n"
      ) << actual2;

      my::hijack hj3 {std::cout};
      when("three-slot resizes one") = [] {
        ch19::vector<X> v {X{1}, X{2}, X{3}};
        v.resize(1);
        expect(v.capacity() == 3_ul);
        expect(v.size() == 1_ul);

        int sum = 0;
        for (auto i = 0; i < v.size(); ++i)
          sum += v[i].val;
        expect(sum == 1_i) << sum;
      };
      const auto actual3 = hj3.release();
      expect(actual3 ==
        "X(1)\n"
        "X(2)\n"
        "X(3)\n"
        "X()\n"
        "X()\n"
        "X()\n"
        "list 3\n"
        "X::copy(1)\n"
        "X::copy(2)\n"
        "X::copy(3)\n"
        "~X(3)\n"
        "~X(2)\n"
        "~X(1)\n"

        "X()\n"
        "resv 1\n"
        "~X(0)\n"

        "del  3\n"
        "~X(3)\n"
        "~X(2)\n"
        "~X(1)\n"
      ) << actual3;
    };
  };
}
