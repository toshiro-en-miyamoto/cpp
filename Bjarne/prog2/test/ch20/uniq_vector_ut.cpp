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
}
