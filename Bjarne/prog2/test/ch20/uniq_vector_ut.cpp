#include <ch20/int_char.h>
#include <ch20/uniq_vector.h>
#include <boost/ut.hpp>
#include <my/hijack.h>
#include <numeric>

int main()
{
    using namespace boost::ut;
    using namespace boost::ut::bdd;

    "size constructor"_test = [] {
        given("zero-value vector") = [] {
            my::hijack hj {std::cout};
            {
                ch20::vector<int_char> v(0);
                expect(v.capacity() == 0_ul);
                expect(v.size() == 0_ul);
                expect(v.data() == nullptr);
                expect(v.begin() == v.end());
            }
            auto actual = hj.release();
            expect(actual ==
                ""
            ) << actual;
        };
        given("one-default vector") = [] {
            my::hijack hj {std::cout};
            {
                ch20::vector<int_char> v(1);
                expect(v.capacity() == 1_ul);
                expect(v.size() == 0_ul);
                expect(v.data() != nullptr);
                expect(v.begin() == v.end());
            }
            auto actual = hj.release();
            expect(actual ==
                "new  default\n"
                "del  0\n"
            ) << actual;
        };
        given("five-default vector") = [] {
            my::hijack hj {std::cout};
            {
                ch20::vector<int_char> v(5);
                expect(v.capacity() == 5_ul);
                expect(v.size() == 0_ul);
                expect(v.data() != nullptr);
                expect(v.begin() == v.end());
            }
            auto actual = hj.release();
            expect(actual ==
                "new  default\n"
                "new  default\n"
                "new  default\n"
                "new  default\n"
                "new  default\n"
                "del  0\n"
                "del  0\n"
                "del  0\n"
                "del  0\n"
                "del  0\n"
            ) << actual;
        };
    };

    "list constructor"_test = [] {
        given("five-value vector") = [] {
            my::hijack hj {std::cout};
            {
                ch20::vector<int_char> v {
                    {1, 'a'}, {2, 'b'}, {3, 'c'},
                    {4, 'd'}, {5, 'e'}
                };
                expect(v.capacity() == 5_ul);
                expect(v.size() == 5_ul);
                expect(v.data() != nullptr);
                expect(v.begin() != v.end());

                auto sum = std::accumulate(v.begin(), v.end(), 0,
                    [] (int accum, const int_char& ic) {
                        return accum + ic.n;
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
                // p = make_unique<int_char>(5)
                "new  default\n"
                "new  default\n"
                "new  default\n"
                "new  default\n"
                "new  default\n"
                // copy(list.begin(), list.end(), p)
                "copy 1 over 0\n"
                "copy 2 over 0\n"
                "copy 3 over 0\n"
                "copy 4 over 0\n"
                "copy 5 over 0\n"
                // literals going out of scope
                "del  5\n"
                "del  4\n"
                "del  3\n"
                "del  2\n"
                "del  1\n"
                // v going out of scope
                "del  5\n"
                "del  4\n"
                "del  3\n"
                "del  2\n"
                "del  1\n"
            ) << actual;
        };
    };

    "reserve"_test = [] {
        given("three-slot vector") = [] {
            when("to six-slot") = [] {
                my::hijack hj {std::cout};
                {
                    ch20::vector<int_char> v(3);
                    v.reserve(6);
                    expect(v.capacity() == 6_ul);
                    expect(v.size() == 0_ul);
                    expect(v.data() != nullptr);
                }
                auto actual = hj.release();
                expect(actual ==
                    // v
                    "new  default\n"
                    "new  default\n"
                    "new  default\n"
                    // reserve(6)
                    "new  default\n"
                    "new  default\n"
                    "new  default\n"
                    "new  default\n"
                    "new  default\n"
                    "new  default\n"
                    // lhs.swap(*this)
                    "del  0\n"
                    "del  0\n"
                    "del  0\n"
                    // v going out of scope
                    "del  0\n"
                    "del  0\n"
                    "del  0\n"
                    "del  0\n"
                    "del  0\n"
                    "del  0\n"
                ) << actual;
            };
            when("to one-slot") = [] {
                my::hijack hj {std::cout};
                {
                    ch20::vector<int_char> v(3);
                    v.reserve(1);
                    expect(v.capacity() == 3_ul);
                    expect(v.size() == 0_ul);
                    expect(v.data() != nullptr);
                }
                auto actual = hj.release();
                expect(actual ==
                    // v
                    "new  default\n"
                    "new  default\n"
                    "new  default\n"
                    // reserve(1)
                    // v going out of scope
                    "del  0\n"
                    "del  0\n"
                    "del  0\n"
                ) << actual;
            };
            when("to zero-slot") = [] {
                my::hijack hj {std::cout};
                {
                    ch20::vector<int_char> v(3);
                    v.reserve(0);
                    expect(v.capacity() == 3_ul);
                    expect(v.size() == 0_ul);
                    expect(v.data() != nullptr);
                }
                auto actual = hj.release();
                expect(actual ==
                    // v
                    "new  default\n"
                    "new  default\n"
                    "new  default\n"
                    // reserve(0)
                    // v going out of scope
                    "del  0\n"
                    "del  0\n"
                    "del  0\n"
                ) << actual;
            };
        };
        given("zero-slot vector") = [] {
            when("to three-slot") = [] {
                my::hijack hj {std::cout};
                {
                    ch20::vector<int_char> v(0);
                    v.reserve(3);
                    expect(v.capacity() == 3_ul);
                    expect(v.size() == 0_ul);
                    expect(v.data() != nullptr);
                }
                auto actual = hj.release();
                expect(actual ==
                    // v
                    // reserve(3)
                    "new  default\n"
                    "new  default\n"
                    "new  default\n"
                    // lhs.swap(*this)
                    // v going out of scope
                    "del  0\n"
                    "del  0\n"
                    "del  0\n"
                ) << actual;
            };
            when("to six-slot") = [] {
                my::hijack hj {std::cout};
                {
                    ch20::vector<int_char> v(0);
                    v.reserve(6);
                    expect(v.capacity() == 6_ul);
                    expect(v.size() == 0_ul);
                    expect(v.data() != nullptr);
                }
                auto actual = hj.release();
                expect(actual ==
                    // v
                    // reserve(6)
                    "new  default\n"
                    "new  default\n"
                    "new  default\n"
                    "new  default\n"
                    "new  default\n"
                    "new  default\n"
                    // lhs.swap(*this)
                    // v going out of scope
                    "del  0\n"
                    "del  0\n"
                    "del  0\n"
                    "del  0\n"
                    "del  0\n"
                    "del  0\n"
                ) << actual;
            };
            when("to zero-slot") = [] {
                my::hijack hj {std::cout};
                {
                    ch20::vector<int_char> v(0);
                    v.reserve(0);
                    expect(v.capacity() == 0_ul);
                    expect(v.size() == 0_ul);
                    expect(v.data() == nullptr);
                }
                auto actual = hj.release();
                expect(actual ==
                    ""
                ) << actual;
            };
        };

        given("three-value vecto") = [] {
            when("to six-slot") = [] {
                my::hijack hj {std::cout};
                {
                    ch20::vector<int_char> v {
                        {1, 'a'}, {2, 'b'}, {3, 'c'}
                    };
                    v.reserve(6);
                    expect(v.capacity() == 6_ul);
                    expect(v.size() == 3_ul);
                    expect(v.data() != nullptr);
                    auto sum = std::accumulate(
                        v.begin(), v.end(), 0,
                        [] (int accum, const int_char& ic) {
                            return accum + ic.n;
                        }
                    );
                    expect(sum == 6_i) << sum;
                }
                auto actual = hj.release();
                expect(actual ==
                    // literals
                    "new  1\n"
                    "new  2\n"
                    "new  3\n"
                    // vector<T> v {new_capa}
                    "new  default\n"
                    "new  default\n"
                    "new  default\n"
                    // copy(lst.b, lst.e, lhs.b)
                    "copy 1 over 0\n"
                    "copy 2 over 0\n"
                    "copy 3 over 0\n"
                    // literals going out of scope
                    "del  3\n"
                    "del  2\n"
                    "del  1\n"
                    // reserve(6)
                        // vector lhs(6)
                        "new  default\n"
                        "new  default\n"
                        "new  default\n"
                        "new  default\n"
                        "new  default\n"
                        "new  default\n"
                        // copy(b, e, lhs.b)
                        "copy 1 over 0\n"
                        "copy 2 over 0\n"
                        "copy 3 over 0\n"
                        // lhs.swap(*this)
                        "del  3\n"
                        "del  2\n"
                        "del  1\n"
                    // v going out of scope
                    "del  0\n"
                    "del  0\n"
                    "del  0\n"
                    "del  3\n"
                    "del  2\n"
                    "del  1\n"
                ) << actual;
            };
            when("to one-slot") = [] {
                my::hijack hj {std::cout};
                {
                    ch20::vector<int_char> v {
                        {1, 'a'}, {2, 'b'}, {3, 'c'}
                    };
                    v.reserve(1);
                    expect(v.capacity() == 3_ul);
                    expect(v.size() == 1_ul);
                    expect(v.data() != nullptr);
                    auto sum = std::accumulate(
                        v.begin(), v.end(), 0,
                        [] (int accum, const int_char& ic) {
                            return accum + ic.n;
                        }
                    );
                    expect(sum == 1_i) << sum;
                }
                auto actual = hj.release();
                expect(actual ==
                    // literals
                    "new  1\n"
                    "new  2\n"
                    "new  3\n"
                    // vector<T> v {new_capa}
                    "new  default\n"
                    "new  default\n"
                    "new  default\n"
                    // copy(lst.b, lst.e, lhs.b)
                    "copy 1 over 0\n"
                    "copy 2 over 0\n"
                    "copy 3 over 0\n"
                    // literals going out of scope
                    "del  3\n"
                    "del  2\n"
                    "del  1\n"
                    // reserve(1)
                    // v going out of scope
                    "del  3\n"
                    "del  2\n"
                    "del  1\n"
                ) << actual;
            };
            when("to zero-slot") = [] {
                my::hijack hj {std::cout};
                {
                    ch20::vector<int_char> v {
                        {1, 'a'}, {2, 'b'}, {3, 'c'}
                    };
                    v.reserve(0);
                    expect(v.capacity() == 3_ul);
                    expect(v.size() == 0_ul);
                    expect(v.data() != nullptr);
                    auto sum = std::accumulate(
                        v.begin(), v.end(), 0,
                        [] (int accum, const int_char& ic) {
                            return accum + ic.n;
                        }
                    );
                    expect(sum == 0_i) << sum;
                }
                auto actual = hj.release();
                expect(actual ==
                    // literals
                    "new  1\n"
                    "new  2\n"
                    "new  3\n"
                    // vector<T> v {new_capa}
                    "new  default\n"
                    "new  default\n"
                    "new  default\n"
                    // copy(lst.b, lst.e, lhs.b)
                    "copy 1 over 0\n"
                    "copy 2 over 0\n"
                    "copy 3 over 0\n"
                    // literals going out of scope
                    "del  3\n"
                    "del  2\n"
                    "del  1\n"
                    // reserve(0)
                    // v going out of scope
                    "del  3\n"
                    "del  2\n"
                    "del  1\n"
                ) << actual;
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
                    expect(v.capacity() == 4_ul);
                    expect(v.size() == 1_ul);
                    expect(v[0].n == 1_i);
                }
                auto actual = hj.release();
                expect(actual ==
                    // literal
                    "new  1\n"
                    // reserve(4)
                    "new  default\n"
                    "new  default\n"
                    "new  default\n"
                    "new  default\n"
                    // copy(b,e,lhs.b)
                    "copy 1 over 0\n"
                    // literal going out of scope
                    "del  1\n"
                    // v going out of scope
                    "del  0\n"
                    "del  0\n"
                    "del  0\n"
                    "del  1\n"
                ) << actual;
            };
            when("push back five values") = [] {
                my::hijack hj {std::cout};
                {
                    ch20::vector<int_char> v(0);
                    for (int i = 0; i < 5; ++i) {
                        v.push_back({1 + i, 'a'});
                    }
                    expect(v.capacity() == 8_ul);
                    expect(v.size() == 5_ul);
                    expect(v[4].n == 5_i);
                }
                auto actual = hj.release();
                expect(actual ==
                    // push_back(1)
                    "new  1\n"
                        // reserve(4)
                        "new  default\n"
                        "new  default\n"
                        "new  default\n"
                        "new  default\n"
                    "copy 1 over 0\n"
                    "del  1\n"
                    // push_back(2)
                    "new  2\n"
                    "copy 2 over 0\n"
                    "del  2\n"
                    // push_back(3)
                    "new  3\n"
                    "copy 3 over 0\n"
                    "del  3\n"
                    // push_back(4)
                    "new  4\n"
                    "copy 4 over 0\n"
                    "del  4\n"
                    // push_back(5)
                    "new  5\n"
                        // reserve(8)
                        "new  default\n"
                        "new  default\n"
                        "new  default\n"
                        "new  default\n"
                        "new  default\n"
                        "new  default\n"
                        "new  default\n"
                        "new  default\n"
                        "copy 1 over 0\n"
                        "copy 2 over 0\n"
                        "copy 3 over 0\n"
                        "copy 4 over 0\n"
                        // detroying 4-slot vector
                        "del  4\n"
                        "del  3\n"
                        "del  2\n"
                        "del  1\n"
                    "copy 5 over 0\n"
                    "del  5\n"
                    // v going out of scope
                    "del  0\n"
                    "del  0\n"
                    "del  0\n"
                    "del  5\n"
                    "del  4\n"
                    "del  3\n"
                    "del  2\n"
                    "del  1\n"
                ) << actual;
            };
        };
        given("one-value vector") = [] {
            when("pushing back three values") = [] {
                my::hijack hj {std::cout};
                {
                    ch20::vector<int_char> v {
                        {1, 'a'}
                    };
                    for (int i = 0; i < 3; ++i) {
                        v.push_back({2 + i, 'a'});
                    }
                    expect(v.capacity() == 4_ul);
                    expect(v.size() == 4_ul);
                    expect(v[3].n == 4_i);
                }
                auto actual = hj.release();
                expect(actual ==
                    // literal
                    "new  1\n"
                    // list constructor
                    "new  default\n"
                    "copy 1 over 0\n"
                    // literal going out of scope
                    "del  1\n"
                    // push_back(2)
                    "new  2\n"
                        // reserve(2*capa_ = 2)
                        "new  default\n"
                        "new  default\n"
                        "copy 1 over 0\n"
                        // lhs.swap(*this)
                        "del  1\n"
                    "copy 2 over 0\n"
                    "del  2\n"
                    // push_back(3)
                    "new  3\n"
                        // reserve(2*capa_ = 4)
                        "new  default\n"
                        "new  default\n"
                        "new  default\n"
                        "new  default\n"
                        "copy 1 over 0\n"
                        "copy 2 over 0\n"
                        // lhs.swap(*this)
                        "del  2\n"
                        "del  1\n"
                    "copy 3 over 0\n"
                    "del  3\n"
                    "new  4\n"
                    "copy 4 over 0\n"
                    "del  4\n"
                    // v goind out of scope
                    "del  4\n"
                    "del  3\n"
                    "del  2\n"
                    "del  1\n"
                ) << actual;
            };
            when("pushing back four values") = [] {
                my::hijack hj {std::cout};
                {
                    ch20::vector<int_char> v {
                        {1, 'a'}
                    };
                    for (int i = 0; i < 4; ++i) {
                        v.push_back({2 + i, 'a'});
                    }
                    expect(v.capacity() == 8_ul);
                    expect(v.size() == 5_ul);
                    expect(v[4].n == 5_i);
                }
                auto actual = hj.release();
                expect(actual ==
                    // literal
                    "new  1\n"
                    // list constructor
                    "new  default\n"
                    "copy 1 over 0\n"
                    // literal going out of scope
                    "del  1\n"
                    // push_back(2)
                    "new  2\n"
                        // reserve(2*capa_ = 2)
                        "new  default\n"
                        "new  default\n"
                        "copy 1 over 0\n"
                        // lhs.swap(*this)
                        "del  1\n"
                    "copy 2 over 0\n"
                    "del  2\n"
                    // push_back(3)
                    "new  3\n"
                        // reserve(2*capa_ = 4)
                        "new  default\n"
                        "new  default\n"
                        "new  default\n"
                        "new  default\n"
                        "copy 1 over 0\n"
                        "copy 2 over 0\n"
                        // lhs.swap(*this)
                        "del  2\n"
                        "del  1\n"
                    "copy 3 over 0\n"
                    "del  3\n"
                    "new  4\n"
                    "copy 4 over 0\n"
                    "del  4\n"
                    // push_back(5)
                    "new  5\n"
                        // reserve(2*capa_ = 8)
                        "new  default\n"
                        "new  default\n"
                        "new  default\n"
                        "new  default\n"
                        "new  default\n"
                        "new  default\n"
                        "new  default\n"
                        "new  default\n"
                        "copy 1 over 0\n"
                        "copy 2 over 0\n"
                        "copy 3 over 0\n"
                        "copy 4 over 0\n"
                        // lhs.swap(*this)
                        "del  4\n"
                        "del  3\n"
                        "del  2\n"
                        "del  1\n"
                    "copy 5 over 0\n"
                    "del  5\n"
                    // v goind out of scope
                    "del  0\n"
                    "del  0\n"
                    "del  0\n"
                    "del  5\n"
                    "del  4\n"
                    "del  3\n"
                    "del  2\n"
                    "del  1\n"
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
                    // list ctor
                    "new  default\n"
                    "new  default\n"
                    "new  default\n"
                    "new  default\n"
                    "new  default\n"
                    "copy 1 over 0\n"
                    "copy 2 over 0\n"
                    "copy 3 over 0\n"
                    "copy 4 over 0\n"
                    "copy 5 over 0\n"
                    // literals going out of scope
                    "del  5\n"
                    "del  4\n"
                    "del  3\n"
                    "del  2\n"
                    "del  1\n"
                    // copying from v5 to v
                    "new  default\n"
                    "new  default\n"
                    "new  default\n"
                    "new  default\n"
                    "new  default\n"
                    "copy 1 over 0\n"
                    "copy 2 over 0\n"
                    "copy 3 over 0\n"
                    "copy 4 over 0\n"
                    "copy 5 over 0\n"
                    // vector v5 going out of scope
                    "del  5\n"
                    "del  4\n"
                    "del  3\n"
                    "del  2\n"
                    "del  1\n"
                    // vector v going out of scope
                    "del  5\n"
                    "del  4\n"
                    "del  3\n"
                    "del  2\n"
                    "del  1\n"
                ) << actual;
            };
        };
    };

    "copy assign value"_test = [] {
        given("three-slot vector") = [] {
            when("from five-slot") = [] {
                my::hijack hj {std::cout};
                {
                    ch20::vector<int_char> v {
                        {1, 'a'}, {2, 'b'}, {3, 'c'}
                    };
                    ch20::vector<int_char> v5 {
                        {4, 'd'}, {5, 'e'}, {6, 'f'},
                        {7, 'g'}, {8, 'h'}
                    };
                    v = v5;
                    expect(v.capacity() == 5_ul);
                    expect(v.size() == 5_ul);
                    int sum = std::accumulate(
                        v.begin(), v.end(), 0,
                        [] (int a, const int_char& ic) {
                            return a += ic.n;
                        }
                    );
                    expect(sum == 30_i);
                }
                auto actual = hj.release();
                expect(actual ==
                    // literals
                    "new  1\n"
                    "new  2\n"
                    "new  3\n"
                    // list ctor v
                    "new  default\n"
                    "new  default\n"
                    "new  default\n"
                    "copy 1 over 0\n"
                    "copy 2 over 0\n"
                    "copy 3 over 0\n"
                    // literals going out of scope
                    "del  3\n"
                    "del  2\n"
                    "del  1\n"
                    // literals
                    "new  4\n"
                    "new  5\n"
                    "new  6\n"
                    "new  7\n"
                    "new  8\n"
                    // list ctor v5
                    "new  default\n"
                    "new  default\n"
                    "new  default\n"
                    "new  default\n"
                    "new  default\n"
                    "copy 4 over 0\n"
                    "copy 5 over 0\n"
                    "copy 6 over 0\n"
                    "copy 7 over 0\n"
                    "copy 8 over 0\n"
                    // literals going out of scope
                    "del  8\n"
                    "del  7\n"
                    "del  6\n"
                    "del  5\n"
                    "del  4\n"
                    // v = v5
                        // vector copy(rhs)
                        "new  default\n"
                        "new  default\n"
                        "new  default\n"
                        "new  default\n"
                        "new  default\n"
                        "copy 4 over 0\n"
                        "copy 5 over 0\n"
                        "copy 6 over 0\n"
                        "copy 7 over 0\n"
                        "copy 8 over 0\n"
                        // rhs.swap(*this)
                        "del  3\n"
                        "del  2\n"
                        "del  1\n"
                    // vector v5 going out of scope
                    "del  8\n"
                    "del  7\n"
                    "del  6\n"
                    "del  5\n"
                    "del  4\n"
                    // vector v going out of scope
                    "del  8\n"
                    "del  7\n"
                    "del  6\n"
                    "del  5\n"
                    "del  4\n"
                ) << actual;
            };
            when("from one-slot") = [] {
                my::hijack hj {std::cout};
                {
                    ch20::vector<int_char> v {
                        {1, 'a'}, {2, 'b'}, {3, 'c'}
                    };
                    ch20::vector<int_char> v1 {
                        {4, 'd'}
                    };
                    v = v1;
                    expect(v.capacity() == 3_ul);
                    expect(v.size() == 1_ul);
                    expect(v[0].n == 4_i);
                }
                auto actual = hj.release();
                expect(actual ==
                    // literals
                    "new  1\n"
                    "new  2\n"
                    "new  3\n"
                    // list ctor v
                    "new  default\n"
                    "new  default\n"
                    "new  default\n"
                    "copy 1 over 0\n"
                    "copy 2 over 0\n"
                    "copy 3 over 0\n"
                    // literals going out of scope
                    "del  3\n"
                    "del  2\n"
                    "del  1\n"
                    // literal
                    "new  4\n"
                    // list ctor v1
                    "new  default\n"
                    "copy 4 over 0\n"
                    // litral going out of scope
                    "del  4\n"
                    // v = v1
                    "copy 4 over 1\n"
                    // v1 going out of scope
                    "del  4\n"
                    // v going out of scope
                    "del  3\n"
                    "del  2\n"
                    "del  4\n"
                ) << actual;
            };
            when("from zero-slot") = [] {
                my::hijack hj {std::cout};
                {
                    ch20::vector<int_char> v {
                        {1, 'a'}, {2, 'b'}, {3, 'c'}
                    };
                    ch20::vector<int_char> v0(0);
                    v = v0;
                    expect(v.capacity() == 3_ul);
                    expect(v.size() == 0_ul);
                    expect(v.data() != nullptr);
                }
                auto actual = hj.release();
                expect(actual ==
                    // literals
                    "new  1\n"
                    "new  2\n"
                    "new  3\n"
                    // list ctor v
                    "new  default\n"
                    "new  default\n"
                    "new  default\n"
                    "copy 1 over 0\n"
                    "copy 2 over 0\n"
                    "copy 3 over 0\n"
                    // literals going out of scope
                    "del  3\n"
                    "del  2\n"
                    "del  1\n"
                    // zero-slot vector
                    // copying from v0 to v
                    // vector v0 going out of scope
                    // vector v going out of scope
                    "del  3\n"
                    "del  2\n"
                    "del  1\n"
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
                    // list ctor v5
                    "new  default\n"
                    "new  default\n"
                    "new  default\n"
                    "new  default\n"
                    "new  default\n"
                    "copy 1 over 0\n"
                    "copy 2 over 0\n"
                    "copy 3 over 0\n"
                    "copy 4 over 0\n"
                    "copy 5 over 0\n"
                    // literals going out of scope
                    "del  5\n"
                    "del  4\n"
                    "del  3\n"
                    "del  2\n"
                    "del  1\n"
                    // moving from v5 to v
                    // v5 going out of scope
                    // v going out of scope
                    "del  5\n"
                    "del  4\n"
                    "del  3\n"
                    "del  2\n"
                    "del  1\n"
                ) << actual;
            };
        };
    };

    "move assign value"_test = [] {
        given("three-value vector") = [] {
            when("from five-value") = [] {
                my::hijack hj {std::cout};
                {
                    ch20::vector<int_char> v {
                        {1, 'a'}, {2, 'b'}, {3, 'c'}
                    };
                    ch20::vector<int_char> v5 {
                        {4, 'd'}, {5, 'e'}, {6, 'f'},
                        {7, 'g'}, {8, 'h'}
                    };
                    v = std::move(v5);
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
                    expect(sum == 30_i);
                }
                auto actual = hj.release();
                expect(actual ==
                    // literals
                    "new  1\n"
                    "new  2\n"
                    "new  3\n"
                    // lit ctor v
                    "new  default\n"
                    "new  default\n"
                    "new  default\n"
                    "copy 1 over 0\n"
                    "copy 2 over 0\n"
                    "copy 3 over 0\n"
                    // literals going out of scope
                    "del  3\n"
                    "del  2\n"
                    "del  1\n"
                    // literals
                    "new  4\n"
                    "new  5\n"
                    "new  6\n"
                    "new  7\n"
                    "new  8\n"
                    // list ctor v5
                    "new  default\n"
                    "new  default\n"
                    "new  default\n"
                    "new  default\n"
                    "new  default\n"
                    "copy 4 over 0\n"
                    "copy 5 over 0\n"
                    "copy 6 over 0\n"
                    "copy 7 over 0\n"
                    "copy 8 over 0\n"
                    // literals going out of scope
                    "del  8\n"
                    "del  7\n"
                    "del  6\n"
                    "del  5\n"
                    "del  4\n"
                    // v = move(v5)
                    // swap() destoying values in v
                    "del  3\n"
                    "del  2\n"
                    "del  1\n"
                    // vector v5 going out of scope
                    // vector v going out of scope
                    "del  8\n"
                    "del  7\n"
                    "del  6\n"
                    "del  5\n"
                    "del  4\n"
                ) << actual;
            };
            when("from one-slot") = [] {
                my::hijack hj {std::cout};
                {
                    ch20::vector<int_char> v {
                        {1, 'a'}, {2, 'b'}, {3, 'c'}
                    };
                    ch20::vector<int_char> v1 {
                        {4, 'd'}
                    };
                    v = std::move(v1);
                    expect(v1.capacity() == 0_ul);
                    expect(v1.size() == 0_ul);
                    expect(v1.data() == nullptr);
                    expect(v.capacity() == 1_ul);
                    expect(v.size() == 1_ul);
                    expect(v[0].n == 4_i);
                }
                auto actual = hj.release();
                expect(actual ==
                    // literals
                    "new  1\n"
                    "new  2\n"
                    "new  3\n"
                    // list ctor v
                    "new  default\n"
                    "new  default\n"
                    "new  default\n"
                    "copy 1 over 0\n"
                    "copy 2 over 0\n"
                    "copy 3 over 0\n"
                    // literals going out of scope
                    "del  3\n"
                    "del  2\n"
                    "del  1\n"
                    // literals
                    "new  4\n"
                    // list ctor v1
                    "new  default\n"
                    "copy 4 over 0\n"
                    // literals going out of scope
                    "del  4\n"
                    // v = move(v1)
                    "del  3\n"
                    "del  2\n"
                    "del  1\n"
                    // vector v1 going out of scope
                    // vector v going out of scope
                    "del  4\n"
                ) << actual;
            };
            when("from zero-slot") = [] {
                my::hijack hj {std::cout};
                {
                    ch20::vector<int_char> v {
                        {1, 'a'}, {2, 'b'}, {3, 'c'}
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
                    // list ctor v
                    "new  default\n"
                    "new  default\n"
                    "new  default\n"
                    "copy 1 over 0\n"
                    "copy 2 over 0\n"
                    "copy 3 over 0\n"
                    // literals going out of scope
                    "del  3\n"
                    "del  2\n"
                    "del  1\n"
                    // zero-slot vector v0
                    // v = move(v0)
                    "del  3\n"
                    "del  2\n"
                    "del  1\n"
                    // vector v0 going out of scope
                    // vector v going out of scope
                ) << actual;
            };
        };
    };
}
