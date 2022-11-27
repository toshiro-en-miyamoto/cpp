#include <ch19/vector.h>
#include <boost/ut.hpp>
#include <iostream>
#include <sstream>
#include <vector>

struct int_char
{
    int n;
    char c;
};

int main()
{
    using namespace boost::ut;
    using namespace boost::ut::bdd;

    suite mem_test = [] {
        "default constructor"_test = [] {
            given("a default mem") = [] {
                std::ostringstream local;
                auto cout_buff = std::cout.rdbuf();
                std::cout.rdbuf(local.rdbuf());

                when("is constructed") = [] {
                    ch19::mem<int_char> m;
                    expect(m.elem() == nullptr);
                    expect(m.capacity() == 0_ul);
                };

                std::cout.rdbuf(cout_buff);
                expect(local.str() ==
                    "new  0\n"
                    "del  0\n"
                ) << local.str();
            };
        };
        "reserve"_test = [] {
            given("a mem with two slots") = [] {
                std::ostringstream local;
                auto cout_buff = std::cout.rdbuf();
                std::cout.rdbuf(local.rdbuf());

                when("reserves three slots") = [] {
                    ch19::mem<int_char> m(2);
                    m.reserve(3);
                    expect(m.elem() != nullptr);
                    expect(m.capacity() == 3_ul);
                };

                std::cout.rdbuf(cout_buff);
                expect(local.str() ==
                    "size 2\n"
                    "resv 2:3\n"
                    "new  3\n"  // mem new_mem {new_p, 3}
                                // swap(new_mem, *this)
                    "mtor 3\n"  //      mem tmp {move(new_mem)}
                    "move 2\n"  //      new_mem = move(*this)
                    "move 3\n"  //      *this = move(tmp)
                    "del  0\n"  //      deleting tmp
                    "del  2\n"  // deleting new_mem (formally *this)
                    "del  3\n"  // deleting m (oramlly new_mem)
                ) << local.str();
            };
        };
        "copy to shorter"_test = [] {
            given("four-slot mem") = [] {
                std::ostringstream local;
                auto cout_buff = std::cout.rdbuf();
                std::cout.rdbuf(local.rdbuf());

                when("copied to two-slot mem") = [] {
                    ch19::mem<int_char> m1(4);
                    ch19::mem<int_char> m2(2);
                    m2 = m1;
                    expect(m2.capacity() == 4_ul);
                };

                std::cout.rdbuf(cout_buff);
                expect(local.str() ==
                    "size 4\n"      // ch19::mem<int_char> m1(4);
                    "size 2\n"      // ch19::mem<int_char> m2(2);
                    "copy 4:2\n"    // copy aissgnment called
                    "new  4\n"      //    mem new_mem {new_p, 4}
                                    //    swap(new_mem, *this)
                    "mtor 4\n"      //      mem tmp {move(new_mem)}
                    "move 2\n"      //      new_mem = move(*this)
                    "move 4\n"      //      *this = move(tmp)
                    "del  0\n"      //      deletint tmp
                    "del  2\n"      //    deleting new_mem (old m2(2))
                    "del  4\n"      // deleting m2
                    "del  4\n"      // deleting m1
                ) << local.str();
            };
        };
        "copy to longer"_test = [] {
            given("two-slot mem") = [] {
                std::ostringstream local;
                auto cout_buff = std::cout.rdbuf();
                std::cout.rdbuf(local.rdbuf());

                when("copied to four-slot mem") = [] {
                    ch19::mem<int_char> m1(2);
                    ch19::mem<int_char> m2(4);
                    m2 = m1;
                    expect(m2.capacity() == 4_ul);
                };

                std::cout.rdbuf(cout_buff);
                expect(local.str() ==
                    "size 2\n"      // ch19::mem<int_char> m1(2);
                    "size 4\n"      // ch19::mem<int_char> m2(4);
                    "copy 2:4\n"    // copy aissgnment called
                    "del  4\n"      // deleting m2
                    "del  2\n"      // deleting m1
                ) << local.str();
            };
        };
        "copy constructor"_test = [] {
            given("two-slot mem") = [] {
                std::ostringstream local;
                auto cout_buff = std::cout.rdbuf();
                std::cout.rdbuf(local.rdbuf());

                when("copy-constructed") = [] {
                    ch19::mem<int_char> m1(2);
                    auto m2 {m1};
                    expect(m2.capacity() == 2_ul);
                };

                std::cout.rdbuf(cout_buff);
                expect(local.str() ==
                    "size 2\n"
                    "ctor 2\n"
                    "del  2\n"
                    "del  2\n"
                ) << local.str();
            };
        };
    };

    suite vector_test = [] {
        "default constructor"_test = [] {
            given("a default vector") = [] {
                std::ostringstream local;
                auto cout_buff = std::cout.rdbuf();
                std::cout.rdbuf(local.rdbuf());

                when("is constructed") = [] {
                    ch19::vector<int_char> v;
                    expect(v.size() == 0_ul);
                    expect(v.capacity() == 0_ul);
                };

                std::cout.rdbuf(cout_buff);
                expect(local.str() ==
                    "new  0\n"
                    "del  0\n"
                ) << local.str();
            };
        };
        "explicit constructor"_test = [] {
            given("a vector with three slots") = [] {
                std::ostringstream local;
                auto cout_buff = std::cout.rdbuf();
                std::cout.rdbuf(local.rdbuf());

                when("reserves two more slots") = [] {
                    ch19::vector<int_char> v(3);
                    expect(v.size() == 3_ul);
                    expect(v.capacity() == 3_ul);
                    v.reserve(v.capacity() + 2);
                    expect(v.size() == 3_ul);
                    expect(v.capacity() == 5_ul);
                };

                std::cout.rdbuf(cout_buff);
                expect(local.str() ==
                    "size 3\n"
                    "resv 3:5\n"
                    "new  5\n"  // mem new_mem {new_p, 5}
                                // swap(new_mem, *this)
                    "mtor 5\n"  //      mem tmp {move(new_mem)}
                    "move 3\n"  //      new_mem = move(*this)
                    "move 5\n"  //      *this = move(tmp)
                    "del  0\n"  //      deleting tmp
                    "del  3\n"  // deleting new_mem (formally *this)
                    "del  5\n"
                ) << local.str();
            };
        };
        "initializer"_test = [] {
            given("a vector") = [] {
                std::ostringstream local;
                auto cout_buff = std::cout.rdbuf();
                std::cout.rdbuf(local.rdbuf());

                when("constructed with a list") = [] {
                    ch19::vector<int_char> v {
                        {1, 'a'}, { 2, 'b' }, { 3, 'c' }
                    };
                    int sum = 0;
                    for (std::size_t i = 0; i < v.size(); ++i) {
                        auto [num, ch] = v[i];
                        sum += num;
                    }
                    expect(sum == 6_i);
                };

                std::cout.rdbuf(cout_buff);
                expect(local.str() ==
                    "size 3\n"
                    "del  3\n"
                ) << local.str();
            };
        };
        "operator []"_test = [] {
            given("a three-slot vector") = [] {
                std::ostringstream local;
                auto cout_buff = std::cout.rdbuf();
                std::cout.rdbuf(local.rdbuf());

                when("initialized with []") = [] {
                    ch19::vector<int_char> v(3);
                    v[0] = {1, 'a'};
                    v[1] = {2, 'b'};
                    v[2] = {3, 'c'};

                    int sum = 0;
                    for (std::size_t i = 0; i < v.size(); ++i) {
                        auto [num, ch] = v[i];
                        sum += num;
                    }
                    expect(sum == 6_i);
                };

                std::cout.rdbuf(cout_buff);
                expect(local.str() ==
                    "size 3\n"
                    "del  3\n"
                ) << local.str();
            };
        };
    };
}
