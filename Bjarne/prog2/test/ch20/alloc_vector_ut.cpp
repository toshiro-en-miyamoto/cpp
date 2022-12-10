#include "int_char.h"
#include <ch20/alloc_vector.h>
#include <boost/ut.hpp>
#include <my/hijack.h>

int main()
{
    using namespace boost::ut;
    using namespace boost::ut::bdd;

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
}
