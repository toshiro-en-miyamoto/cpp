// g++ vector_ut.cpp -std=c++20
#include "int_char.h"
// #include "alloc_vector.h"
#include "mem_vector.h"
#include <boost/ut.hpp>
#include <my/hijack.h>
#include <numeric>

int main()
{
  using namespace boost::ut;
  using namespace boost::ut::bdd;

  "capacity ctor"_test = [] {
    given("zero-slot vector") = [] {
      when("get capacity and size") = [] {
        ch20::vector<int_char> v(0);
        expect(v.capacity() == 0_ul);
        expect(v.size() == 0_ul);
      };
      when("get pointer") = [] {
        ch20::vector<int_char> v(0);
        expect(v.data() == nullptr);
      };
    };
    given("four-slot vector") = [] {
      when("get capacity and size") = [] {
        ch20::vector<int_char> v(4);
        expect(v.capacity() == 4_ul);
        expect(v.size() == 0_ul);
      };
      when("get pointer") = [] {
        ch20::vector<int_char> v(4);
        expect(v.data() != nullptr);
      };
    };
  };

  "copy ctor"_test = [] {
    given("a new vector") = [] {
      when("from five-slot") = [] {
        ch20::vector<int_char> v5(5);
        ch20::vector<int_char> v {v5};
        expect(v.capacity() == 5_ul);
        expect(v.size() == 0_ul);
        expect(v.data() != nullptr);
        expect(v5.capacity() == 5_ul);
        expect(v5.size() == 0_ul);
        expect(v5.data() != nullptr);
      };
      when("from zero-slot") = [] {
        ch20::vector<int_char> v0(0);
        ch20::vector<int_char> v {v0};
        expect(v.capacity() == 0_ul);
        expect(v.size() == 0_ul);
        expect(v.data() == nullptr);
        expect(v0.capacity() == 0_ul);
        expect(v0.size() == 0_ul);
        expect(v0.data() == nullptr);
      };
    };
  };

  "copy assign"_test = [] {
    given("five-slot vector") = [] {
      when("from nine-slot") = [] {
        ch20::vector<int_char> v(5);
        ch20::vector<int_char> v9(9);
        v = v9;
        expect(v.capacity() == 9_ul);
        expect(v.size() == 0_ul);
        expect(v.data() != nullptr);
        expect(v9.capacity() == 9_ul);
        expect(v9.size() == 0_ul);
        expect(v9.data() != nullptr);
      };
      when("from one-slot") = [] {
        ch20::vector<int_char> v(5);
        ch20::vector<int_char> v1(1);
        v = v1;
        expect(v.capacity() == 5_ul);
        expect(v.size() == 0_ul);
        expect(v.data() != nullptr);
        expect(v1.capacity() == 1_ul);
        expect(v1.size() == 0_ul);
        expect(v1.data() != nullptr);
      };
      when("from zero-slot") = [] {
        ch20::vector<int_char> v(5);
        ch20::vector<int_char> v0(0);
        v = v0;
        expect(v.capacity() == 5_ul);
        expect(v.size() == 0_ul);
        expect(v.data() != nullptr);
        expect(v0.capacity() == 0_ul);
        expect(v0.size() == 0_ul);
        expect(v0.data() == nullptr);
      };
    };
    given("zero-slot vector") = [] {
      when("from nine-slot") = [] {
        ch20::vector<int_char> v(0);
        ch20::vector<int_char> v9(9);
        v = v9;
        expect(v.capacity() == 9_ul);
        expect(v.size() == 0_ul);
        expect(v.data() != nullptr);
        expect(v9.capacity() == 9_ul);
        expect(v9.size() == 0_ul);
        expect(v9.data() != nullptr);
      };
      when("from one-slot") = [] {
        ch20::vector<int_char> v(0);
        ch20::vector<int_char> v1(1);
        v = v1;
        expect(v.capacity() == 1_ul);
        expect(v.size() == 0_ul);
        expect(v.data() != nullptr);
        expect(v1.capacity() == 1_ul);
        expect(v1.size() == 0_ul);
        expect(v1.data() != nullptr);
      };
      when("from zero-slot") = [] {
        ch20::vector<int_char> v(0);
        ch20::vector<int_char> v0(0);
        v = v0;
        expect(v.capacity() == 0_ul);
        expect(v.size() == 0_ul);
        expect(v.data() == nullptr);
        expect(v0.capacity() == 0_ul);
        expect(v0.size() == 0_ul);
        expect(v0.data() == nullptr);
      };
    };
  };

  "move ctor"_test = [] {
    given("a new vector") = [] {
      when("from five-slot") = [] {
        ch20::vector<int_char> v5(5);
        ch20::vector<int_char> v {std::move(v5)};
        expect(v.capacity() == 5_ul);
        expect(v.size() == 0_ul);
        expect(v.data() != nullptr);
        expect(v5.capacity() == 0_ul);
        expect(v5.size() == 0_ul);
        expect(v5.data() == nullptr);
      };
      when("from one-slot") = [] {
        ch20::vector<int_char> v1(1);
        ch20::vector<int_char> v {std::move(v1)};
        expect(v.capacity() == 1_ul);
        expect(v.size() == 0_ul);
        expect(v.data() != nullptr);
        expect(v1.capacity() == 0_ul);
        expect(v1.size() == 0_ul);
        expect(v1.data() == nullptr);
      };
      when("from zero-slot") = [] {
        ch20::vector<int_char> v0(0);
        ch20::vector<int_char> v {std::move(v0)};
        expect(v.capacity() == 0_ul);
        expect(v.size() == 0_ul);
        expect(v.data() == nullptr);
        expect(v0.capacity() == 0_ul);
        expect(v0.size() == 0_ul);
        expect(v0.data() == nullptr);
      };
    };
  };

  "move assign"_test = [] {
    given("five-slot vector") = [] {
      when("from nine-slot") = [] {
        ch20::vector<int_char> v(5);
        ch20::vector<int_char> v9(9);
        v = std::move(v9);
        expect(v.capacity() == 9_ul);
        expect(v.size() == 0_ul);
        expect(v.data() != nullptr);
        expect(v9.capacity() == 0_ul);
        expect(v9.size() == 0_ul);
        expect(v9.data() == nullptr);
      };
      when("from one-slot") = [] {
        ch20::vector<int_char> v(5);
        ch20::vector<int_char> v1(1);
        v = std::move(v1);
        expect(v.capacity() == 1_ul);
        expect(v.size() == 0_ul);
        expect(v.data() != nullptr);
        expect(v1.capacity() == 0_ul);
        expect(v1.size() == 0_ul);
        expect(v1.data() == nullptr);
      };
      when("from zero-slot") = [] {
        ch20::vector<int_char> v(5);
        ch20::vector<int_char> v0(0);
        v = std::move(v0);
        expect(v.capacity() == 0_ul);
        expect(v.size() == 0_ul);
        expect(v.data() == nullptr);
        expect(v0.capacity() == 0_ul);
        expect(v0.size() == 0_ul);
        expect(v0.data() == nullptr);
      };
    };
    given("zero-slot vector") = [] {
      when("from nine-slot") = [] {
        ch20::vector<int_char> v(0);
        ch20::vector<int_char> v9(9);
        v = std::move(v9);
        expect(v.capacity() == 9_ul);
        expect(v.size() == 0_ul);
        expect(v.data() != nullptr);
        expect(v9.capacity() == 0_ul);
        expect(v9.size() == 0_ul);
        expect(v9.data() == nullptr);
      };
      when("from one-slot") = [] {
        ch20::vector<int_char> v(0);
        ch20::vector<int_char> v1(1);
        v = std::move(v1);
        expect(v.capacity() == 1_ul);
        expect(v.size() == 0_ul);
        expect(v.data() != nullptr);
        expect(v1.capacity() == 0_ul);
        expect(v1.size() == 0_ul);
        expect(v1.data() == nullptr);
      };
      when("from zero-slot") = [] {
        ch20::vector<int_char> v(0);
        ch20::vector<int_char> v0(0);
        v = std::move(v0);
        expect(v.capacity() == 0_ul);
        expect(v.size() == 0_ul);
        expect(v.data() == nullptr);
        expect(v0.capacity() == 0_ul);
        expect(v0.size() == 0_ul);
        expect(v0.data() == nullptr);
      };
    };
  };

  "reserve"_test = [] {
    given("five-slot vector") = [] {
      when("to nine-slot") = [] {
        ch20::vector<int_char> v(5);
        v.reserve(9);
        expect(v.capacity() == 9_ul);
        expect(v.size() == 0_ul);
        expect(v.data() != nullptr);
      };
      when("to one-slot") = [] {
        ch20::vector<int_char> v(5);
        v.reserve(1);
        expect(v.capacity() == 5_ul);
        expect(v.size() == 0_ul);
        expect(v.data() != nullptr);
      };
      when("to zero-slot") = [] {
        ch20::vector<int_char> v(5);
        v.reserve(0);
        expect(v.capacity() == 5_ul);
        expect(v.size() == 0_ul);
        expect(v.data() != nullptr);
      };
    };
    given("zero-slot vector") = [] {
      when("to nine-slot") = [] {
        ch20::vector<int_char> v(0);
        v.reserve(9);
        expect(v.capacity() == 9_ul);
        expect(v.size() == 0_ul);
        expect(v.data() != nullptr);
      };
      when("to one-slot") = [] {
        ch20::vector<int_char> v(0);
        v.reserve(1);
        expect(v.capacity() == 1_ul);
        expect(v.size() == 0_ul);
        expect(v.data() != nullptr);
      };
      when("to zero-slot") = [] {
        ch20::vector<int_char> v(0);
        v.reserve(0);
        expect(v.capacity() == 0_ul);
        expect(v.size() == 0_ul);
        expect(v.data() == nullptr);
      };
    };
  };

  "push_back"_test = [] {
    given("zero-slot vector") = [] {
      when("push back one value") = [] {
        my::hijack hj {std::cout};
        {
          ch20::vector<int_char> v(0);
          int_char value {1, 'a'};
          v.push_back(value);
          expect(v.capacity() == 8_ul);
          expect(v.size() == 1_ul);
          expect(v[0].n == 1_i);
        }
        auto actual = hj.release();
        expect(actual ==
          "new  1\n"
          "ctor 1\n"
          "del  1\n"
          "del  1\n"
        ) << actual;
      };
      when("push back two values") = [] {
        my::hijack hj {std::cout};
        {
          ch20::vector<int_char> v(0);
          for (int i = 0; i < 2; ++i) {
            v.push_back({1 + i, 'a'});
          }
          expect(v.capacity() == 8_ul);
          expect(v.size() == 2_ul);
          expect(v[1].n == 2_i);
        }
        auto actual = hj.release();
        expect(actual ==
          // push_back()
          "new  1\n"
          "ctor 1\n"
          "del  1\n"
          // push_back()
          "new  2\n"
          "ctor 2\n"
          "del  2\n"
          // going out of scope
          "del  1\n"
          "del  2\n"
        ) << actual;
      };
      when("push back nine values") = [] {
        my::hijack hj {std::cout};
        {
          ch20::vector<int_char> v(0);
          for (int i = 0; i < 9; ++i) {
            v.push_back({1 + i, 'a'});
          }
          expect(v.capacity() == 16_ul);
          expect(v.size() == 9_ul);
          expect(v[8].n == 9_i);
        }
        auto actual = hj.release();
        expect(actual ==
          // push_back()
          "new  1\n"
          "ctor 1\n"
          "del  1\n"
          // push_back()
          "new  2\n"
          "ctor 2\n"
          "del  2\n"
          // push_back()
          "new  3\n"
          "ctor 3\n"
          "del  3\n"
          // push_back()
          "new  4\n"
          "ctor 4\n"
          "del  4\n"
          // push_back()
          "new  5\n"
          "ctor 5\n"
          "del  5\n"
          // push_back()
          "new  6\n"
          "ctor 6\n"
          "del  6\n"
          // push_back()
          "new  7\n"
          "ctor 7\n"
          "del  7\n"
          // push_back()
          "new  8\n"
          "ctor 8\n"
          "del  8\n"
          // push_back()
          "new  9\n"
            // reserve(8)
            "ctor 1\n"
            "ctor 2\n"
            "ctor 3\n"
            "ctor 4\n"
            "ctor 5\n"
            "ctor 6\n"
            "ctor 7\n"
            "ctor 8\n"
            "del  1\n"
            "del  2\n"
            "del  3\n"
            "del  4\n"
            "del  5\n"
            "del  6\n"
            "del  7\n"
            "del  8\n"
          "ctor 9\n"
          "del  9\n"
          // going out of scope
          "del  1\n"
          "del  2\n"
          "del  3\n"
          "del  4\n"
          "del  5\n"
          "del  6\n"
          "del  7\n"
          "del  8\n"
          "del  9\n"
        ) << actual;
      };
    };

    given("five-slot vector") = [] {
      when("push back one value") = [] {
        my::hijack hj {std::cout};
        {
          ch20::vector<int_char> v(5);
          int_char value {1, 'a'};
          v.push_back(value);
          expect(v.capacity() == 5_ul);
          expect(v.size() == 1_ul);
          expect(v[0].n == 1_i);
        }
        auto actual = hj.release();
        expect(actual ==
          "new  1\n"
          "ctor 1\n"
          "del  1\n"
          "del  1\n"
        ) << actual;
      };
      when("push back two values") = [] {
        my::hijack hj {std::cout};
        {
          ch20::vector<int_char> v(5);
          for (int i = 0; i < 2; ++i) {
            v.push_back({1 + i, 'a'});
          }
          expect(v.capacity() == 5_ul);
          expect(v.size() == 2_ul);
          expect(v[1].n == 2_i);
        }
        auto actual = hj.release();
        expect(actual ==
          // push_back()
          "new  1\n"
          "ctor 1\n"
          "del  1\n"
          // push_back()
          "new  2\n"
          "ctor 2\n"
          "del  2\n"
          // going out of scope
          "del  1\n"
          "del  2\n"
        ) << actual;
      };
      when("push back 12 values") = [] {
        my::hijack hj {std::cout};
        {
          ch20::vector<int_char> v(5);
          for (int i = 0; i < 12; ++i) {
            v.push_back({1 + i, 'a'});
          }
          expect(v.capacity() == 20_ul);
          expect(v.size() == 12_ul);
          expect(v[11].n == 12_i);
        }
        auto actual = hj.release();
        expect(actual ==
          // push_back()
          "new  1\n"
          "ctor 1\n"
          "del  1\n"
          // push_back()
          "new  2\n"
          "ctor 2\n"
          "del  2\n"
          // push_back()
          "new  3\n"
          "ctor 3\n"
          "del  3\n"
          // push_back()
          "new  4\n"
          "ctor 4\n"
          "del  4\n"
          // push_back()
          "new  5\n"
          "ctor 5\n"
          "del  5\n"
          // push_back()
          "new  6\n"
            // reserve(10)
            "ctor 1\n"
            "ctor 2\n"
            "ctor 3\n"
            "ctor 4\n"
            "ctor 5\n"
            "del  1\n"
            "del  2\n"
            "del  3\n"
            "del  4\n"
            "del  5\n"
          "ctor 6\n"
          "del  6\n"
          // push_back()
          "new  7\n"
          "ctor 7\n"
          "del  7\n"
          // push_back()
          "new  8\n"
          "ctor 8\n"
          "del  8\n"
          // push_back()
          "new  9\n"
          "ctor 9\n"
          "del  9\n"
          // push_back()
          "new  10\n"
          "ctor 10\n"
          "del  10\n"
          // push_back()
          "new  11\n"
            // reserve(20)
            "ctor 1\n"
            "ctor 2\n"
            "ctor 3\n"
            "ctor 4\n"
            "ctor 5\n"
            "ctor 6\n"
            "ctor 7\n"
            "ctor 8\n"
            "ctor 9\n"
            "ctor 10\n"
            "del  1\n"
            "del  2\n"
            "del  3\n"
            "del  4\n"
            "del  5\n"
            "del  6\n"
            "del  7\n"
            "del  8\n"
            "del  9\n"
            "del  10\n"
          "ctor 11\n"
          "del  11\n"
          // push_back()
          "new  12\n"
          "ctor 12\n"
          "del  12\n"
          // going out of scope
          "del  1\n"
          "del  2\n"
          "del  3\n"
          "del  4\n"
          "del  5\n"
          "del  6\n"
          "del  7\n"
          "del  8\n"
          "del  9\n"
          "del  10\n"
          "del  11\n"
          "del  12\n"
        ) << actual;
      };
    };
  };

  "list initialization"_test = [] {
    given("zero-value") = [] {
      when("initialized") = [] {
        ch20::vector<int_char> v {};
        expect(v.capacity() == 0_ul);
        expect(v.size() == 0_ul);
        expect(v.data() == nullptr);
      };
    };
    given("five-value") = [] {
      when("initialized") = [] {
        my::hijack hj {std::cout};
        {
          ch20::vector<int_char> v {
            {1, 'a'}, {2, 'b'}, {3, 'c'},
            {4, 'd'}, {5, 'e'}
          };
          expect(v.capacity() == 5_ul);
          expect(v.size() == 5_ul);
          expect(v[4].n == 5_i);
        }
        auto actual = hj.release();
        expect(actual ==
          // literals
          "new  1\n"
          "new  2\n"
          "new  3\n"
          "new  4\n"
          "new  5\n"
          // copied into ptr_
          "ctor 1\n"
          "ctor 2\n"
          "ctor 3\n"
          "ctor 4\n"
          "ctor 5\n"
          // literals going out of scope
          "del  5\n"
          "del  4\n"
          "del  3\n"
          "del  2\n"
          "del  1\n"
          // vector v going out of scope
          "del  1\n"
          "del  2\n"
          "del  3\n"
          "del  4\n"
          "del  5\n"
        ) << actual;
      };
    };
  };

  "copy ctor value"_test = [] {
    given("a new vector") = [] {
      when("from five-values") = [] {
        my::hijack hj {std::cout};
        {
          ch20::vector<int_char> v5 {
            {1, 'a'}, {2, 'b'}, {3, 'c'},
            {4, 'd'}, {5, 'e'}
          };
          ch20::vector<int_char> v {v5};
          expect(v.capacity() == 5_ul);
          expect(v.size() == 5_ul);
          int sum = std::accumulate(
            v.begin(), v.end(), 0,
            [] (int a, const int_char& ic) {
              return a += ic.n;
            }
          );
          expect(sum == 15_i);
        }
        auto actual = hj.release();
        expect(actual ==
          // literals
          "new  1\n"
          "new  2\n"
          "new  3\n"
          "new  4\n"
          "new  5\n"
          // copied into ptr_
          "ctor 1\n"
          "ctor 2\n"
          "ctor 3\n"
          "ctor 4\n"
          "ctor 5\n"
          // literals going out of scope
          "del  5\n"
          "del  4\n"
          "del  3\n"
          "del  2\n"
          "del  1\n"
          // copying from v5 to v
          "ctor 1\n"
          "ctor 2\n"
          "ctor 3\n"
          "ctor 4\n"
          "ctor 5\n"
          // vector v5 going out of scope
          "del  1\n"
          "del  2\n"
          "del  3\n"
          "del  4\n"
          "del  5\n"
          // vector v going out of scope
          "del  1\n"
          "del  2\n"
          "del  3\n"
          "del  4\n"
          "del  5\n"
        ) << actual;
      };
    };
  };

  "copy assign value"_test = [] {
    given("five-slot vector") = [] {
      when("from nine-slot") = [] {
        my::hijack hj {std::cout};
        {
          ch20::vector<int_char> v {
            {1, 'a'}, {2, 'b'}, {3, 'c'},
            {4, 'd'}, {5, 'e'}
          };
          ch20::vector<int_char> v9 {
            { 7, 'g'}, { 8, 'h'}, { 9, 'i'},
            {10, 'j'}, {11, 'k'}, {12, 'l'},
            {13, 'm'}, {14, 'n'}, {15, 'o'}
          };
          v = v9;
          expect(v.capacity() == 9_ul);
          expect(v.size() == 9_ul);
          int sum = std::accumulate(
            v.begin(), v.end(), 0,
            [] (int a, const int_char& ic) {
              return a += ic.n;
            }
          );
          expect(sum == 99_i);
        }
        auto actual = hj.release();
        expect(actual ==
          // literals
          "new  1\n"
          "new  2\n"
          "new  3\n"
          "new  4\n"
          "new  5\n"
          // copied into ptr_ of v
          "ctor 1\n"
          "ctor 2\n"
          "ctor 3\n"
          "ctor 4\n"
          "ctor 5\n"
          // literals going out of scope
          "del  5\n"
          "del  4\n"
          "del  3\n"
          "del  2\n"
          "del  1\n"
          // literals
          "new  7\n"
          "new  8\n"
          "new  9\n"
          "new  10\n"
          "new  11\n"
          "new  12\n"
          "new  13\n"
          "new  14\n"
          "new  15\n"
          // copied into ptr_ of v9
          "ctor 7\n"
          "ctor 8\n"
          "ctor 9\n"
          "ctor 10\n"
          "ctor 11\n"
          "ctor 12\n"
          "ctor 13\n"
          "ctor 14\n"
          "ctor 15\n"
          // literals going out of scope
          "del  15\n"
          "del  14\n"
          "del  13\n"
          "del  12\n"
          "del  11\n"
          "del  10\n"
          "del  9\n"
          "del  8\n"
          "del  7\n"
          // copying from v9 to v
          "ctor 7\n"
          "ctor 8\n"
          "ctor 9\n"
          "ctor 10\n"
          "ctor 11\n"
          "ctor 12\n"
          "ctor 13\n"
          "ctor 14\n"
          "ctor 15\n"
          // swap() destroying values in v
          "del  1\n"
          "del  2\n"
          "del  3\n"
          "del  4\n"
          "del  5\n"
          // vector v9 going out of scope
          "del  7\n"
          "del  8\n"
          "del  9\n"
          "del  10\n"
          "del  11\n"
          "del  12\n"
          "del  13\n"
          "del  14\n"
          "del  15\n"
          // vector v going out of scope
          "del  7\n"
          "del  8\n"
          "del  9\n"
          "del  10\n"
          "del  11\n"
          "del  12\n"
          "del  13\n"
          "del  14\n"
          "del  15\n"
        ) << actual;
      };
      when("from one-slot") = [] {
        my::hijack hj {std::cout};
        {
          ch20::vector<int_char> v {
            {1, 'a'}, {2, 'b'}, {3, 'c'},
            {4, 'd'}, {5, 'e'}
          };
          ch20::vector<int_char> v1 {
            {6, 'f'}
          };
          v = v1;
          expect(v.capacity() == 5_ul);
          expect(v.size() == 1_ul);
          int sum = std::accumulate(
            v.begin(), v.end(), 0,
            [] (int a, const int_char& ic) {
              return a += ic.n;
            }
          );
          expect(sum == 6_i);
        }
        auto actual = hj.release();
        expect(actual ==
          // literals
          "new  1\n"
          "new  2\n"
          "new  3\n"
          "new  4\n"
          "new  5\n"
          // copied into ptr_ of v
          "ctor 1\n"
          "ctor 2\n"
          "ctor 3\n"
          "ctor 4\n"
          "ctor 5\n"
          // literals going out of scope
          "del  5\n"
          "del  4\n"
          "del  3\n"
          "del  2\n"
          "del  1\n"
          // literals
          "new  6\n"
          // copied into ptr_ of v1
          "ctor 6\n"
          // literals going out of scope
          "del  6\n"
          // copying from v1 to v
          "del  1\n"
          "del  2\n"
          "del  3\n"
          "del  4\n"
          "del  5\n"
          "ctor 6\n"
          // vector v1 going out of scope
          "del  6\n"
          // vector v going out of scope
          "del  6\n"
        ) << actual;
      };
      when("from zero-slot") = [] {
        my::hijack hj {std::cout};
        {
          ch20::vector<int_char> v {
            {1, 'a'}, {2, 'b'}, {3, 'c'},
            {4, 'd'}, {5, 'e'}
          };
          ch20::vector<int_char> v0(0);
          v = v0;
          expect(v.capacity() == 5_ul);
          expect(v.size() == 0_ul);
          expect(v.data() != nullptr);
        }
        auto actual = hj.release();
        expect(actual ==
          // literals
          "new  1\n"
          "new  2\n"
          "new  3\n"
          "new  4\n"
          "new  5\n"
          // copied into ptr_ of v
          "ctor 1\n"
          "ctor 2\n"
          "ctor 3\n"
          "ctor 4\n"
          "ctor 5\n"
          // literals going out of scope
          "del  5\n"
          "del  4\n"
          "del  3\n"
          "del  2\n"
          "del  1\n"
          // zero-slot vector
          // copying from v0 to v
          "del  1\n"
          "del  2\n"
          "del  3\n"
          "del  4\n"
          "del  5\n"
          // vector v0 going out of scope
          // vector v going out of scope
        ) << actual;
      };
    };
  };

  "move ctor value"_test = [] {
    given("a new vector") = [] {
      when("from five-slot") = [] {
        my::hijack hj {std::cout};
        {
          ch20::vector<int_char> v5 {
            {1, 'a'}, {2, 'b'}, {3, 'c'},
            {4, 'd'}, {5, 'e'}
          };
          ch20::vector<int_char> v {std::move(v5)};
          expect(v5.capacity() == 0_ul);
          expect(v5.size() == 0_ul);
          expect(v5.data() == nullptr);
          expect(v.capacity() == 5_ul);
          expect(v.size() == 5_ul);
          int sum = std::accumulate(
            v.begin(), v.end(), 0,
            [] (int a, const int_char& ic) {
              return a += ic.n;
            }
          );
          expect(sum == 15_i);
        }
        auto actual = hj.release();
        expect(actual ==
          // literals
          "new  1\n"
          "new  2\n"
          "new  3\n"
          "new  4\n"
          "new  5\n"
          // copied into ptr_ of v5
          "ctor 1\n"
          "ctor 2\n"
          "ctor 3\n"
          "ctor 4\n"
          "ctor 5\n"
          // literals going out of scope
          "del  5\n"
          "del  4\n"
          "del  3\n"
          "del  2\n"
          "del  1\n"
          // moving from v5 to v
          // vector v going out of scope
          "del  1\n"
          "del  2\n"
          "del  3\n"
          "del  4\n"
          "del  5\n"
        ) << actual;
      };
    };
  };

  "move assign value"_test = [] {
    given("five-slot vector") = [] {
      when("from nine-slot") = [] {
        my::hijack hj {std::cout};
        {
          ch20::vector<int_char> v {
            {1, 'a'}, {2, 'b'}, {3, 'c'},
            {4, 'd'}, {5, 'e'}
          };
          ch20::vector<int_char> v9 {
            { 7, 'g'}, { 8, 'h'}, { 9, 'i'},
            {10, 'j'}, {11, 'k'}, {12, 'l'},
            {13, 'm'}, {14, 'n'}, {15, 'o'}
          };
          v = std::move(v9);
          expect(v9.capacity() == 0_ul);
          expect(v9.size() == 0_ul);
          expect(v9.data() == nullptr);
          expect(v.capacity() == 9_ul);
          expect(v.size() == 9_ul);
          int sum = std::accumulate(
            v.begin(), v.end(), 0,
            [] (int a, const int_char& ic) {
              return a += ic.n;
            }
          );
          expect(sum == 99_i);
        }
        auto actual = hj.release();
        expect(actual ==
          // literals
          "new  1\n"
          "new  2\n"
          "new  3\n"
          "new  4\n"
          "new  5\n"
          // copied into ptr_ of v
          "ctor 1\n"
          "ctor 2\n"
          "ctor 3\n"
          "ctor 4\n"
          "ctor 5\n"
          // literals going out of scope
          "del  5\n"
          "del  4\n"
          "del  3\n"
          "del  2\n"
          "del  1\n"
          // literals
          "new  7\n"
          "new  8\n"
          "new  9\n"
          "new  10\n"
          "new  11\n"
          "new  12\n"
          "new  13\n"
          "new  14\n"
          "new  15\n"
          // copied into ptr_ of v9
          "ctor 7\n"
          "ctor 8\n"
          "ctor 9\n"
          "ctor 10\n"
          "ctor 11\n"
          "ctor 12\n"
          "ctor 13\n"
          "ctor 14\n"
          "ctor 15\n"
          // literals going out of scope
          "del  15\n"
          "del  14\n"
          "del  13\n"
          "del  12\n"
          "del  11\n"
          "del  10\n"
          "del  9\n"
          "del  8\n"
          "del  7\n"
          // moving from v9 to v
          // swap() destroying values in v
          "del  1\n"
          "del  2\n"
          "del  3\n"
          "del  4\n"
          "del  5\n"
          // vector v9 going out of scope
          // vector v going out of scope
          "del  7\n"
          "del  8\n"
          "del  9\n"
          "del  10\n"
          "del  11\n"
          "del  12\n"
          "del  13\n"
          "del  14\n"
          "del  15\n"
        ) << actual;
      };
      when("from one-slot") = [] {
        my::hijack hj {std::cout};
        {
          ch20::vector<int_char> v {
            {1, 'a'}, {2, 'b'}, {3, 'c'},
            {4, 'd'}, {5, 'e'}
          };
          ch20::vector<int_char> v1 {
            {6, 'f'}
          };
          v = std::move(v1);
          expect(v1.capacity() == 0_ul);
          expect(v1.size() == 0_ul);
          expect(v1.data() == nullptr);
          expect(v.capacity() == 1_ul);
          expect(v.size() == 1_ul);
          int sum = std::accumulate(
            v.begin(), v.end(), 0,
            [] (int a, const int_char& ic) {
              return a += ic.n;
            }
          );
          expect(sum == 6_i);
        }
        auto actual = hj.release();
        expect(actual ==
          // literals
          "new  1\n"
          "new  2\n"
          "new  3\n"
          "new  4\n"
          "new  5\n"
          // copied into ptr_ of v
          "ctor 1\n"
          "ctor 2\n"
          "ctor 3\n"
          "ctor 4\n"
          "ctor 5\n"
          // literals going out of scope
          "del  5\n"
          "del  4\n"
          "del  3\n"
          "del  2\n"
          "del  1\n"
          // literals
          "new  6\n"
          // copied into ptr_ of v1
          "ctor 6\n"
          // literals going out of scope
          "del  6\n"
          // moving from v1 to v
          "del  1\n"
          "del  2\n"
          "del  3\n"
          "del  4\n"
          "del  5\n"
          // vector v1 going out of scope
          // vector v going out of scope
          "del  6\n"
        ) << actual;
      };
      when("from zero-slot") = [] {
        my::hijack hj {std::cout};
        {
          ch20::vector<int_char> v {
            {1, 'a'}, {2, 'b'}, {3, 'c'},
            {4, 'd'}, {5, 'e'}
          };
          ch20::vector<int_char> v0(0);
          v = std::move(v0);
          expect(v0.capacity() == 0_ul);
          expect(v0.size() == 0_ul);
          expect(v0.data() == nullptr);
          expect(v.capacity() == 0_ul);
          expect(v.size() == 0_ul);
          expect(v.data() == nullptr);
        }
        auto actual = hj.release();
        expect(actual ==
          // literals
          "new  1\n"
          "new  2\n"
          "new  3\n"
          "new  4\n"
          "new  5\n"
          // copied into ptr_ of v
          "ctor 1\n"
          "ctor 2\n"
          "ctor 3\n"
          "ctor 4\n"
          "ctor 5\n"
          // literals going out of scope
          "del  5\n"
          "del  4\n"
          "del  3\n"
          "del  2\n"
          "del  1\n"
          // zero-slot vector
          // moving from v0 to v
          "del  1\n"
          "del  2\n"
          "del  3\n"
          "del  4\n"
          "del  5\n"
          // vector v0 going out of scope
          // vector v going out of scope
        ) << actual;
      };
    };
  };
}
