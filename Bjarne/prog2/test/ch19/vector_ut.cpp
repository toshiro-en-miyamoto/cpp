#include <ch19/vector.h>
#include <boost/ut.hpp>
#include <my/hijack.h>

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
                when("is constructed") = [] {
                    my::hijack hj(std::cout);
                    {
                        ch19::mem<int_char> m;
                        expect(m.elem() == nullptr);
                        expect(m.capacity() == 0_ul);
                    }
                    expect(hj.release() == ""
                        "new  0\n"
                        "del  0\n"
                    );
                };
            };
        };
        "reserve"_test = [] {
            given("a mem with two slots") = [] {
                when("reserves three slots") = [] {
                    my::hijack hj(std::cout);
                    {
                        ch19::mem<int_char> m(2);
                        m.reserve(3);
                        expect(m.elem() != nullptr);
                        expect(m.capacity() == 3_ul);
                    }
                    expect(hj.release() ==
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
                    );
                };
            };
        };
        "copy to shorter"_test = [] {
            given("four-slot mem") = [] {
                when("copied to two-slot mem") = [] {
                    my::hijack hj(std::cout);
                    {
                        ch19::mem<int_char> m1(4);
                        m1[0] = {1, 'a'};
                        m1[1] = {2, 'b'};
                        m1[2] = {3, 'c'};
                        m1[3] = {4, 'd'};
                        ch19::mem<int_char> m2(2);
                        m2[0] = {5, 'e'};
                        m2[1] = {6, 'f'};

                        m2 = m1;
                        expect(m2.capacity() == 4_ul);

                        int sum = 0;
                        for (std::size_t i = 0; i < m2.capacity(); ++i) {
                            sum += m2[i].n;
                        }
                        expect(sum == 10_i);
                    }
                    expect(hj.release() ==
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
                    );
                };
            };
        };
        "copy to longer"_test = [] {
            given("two-slot mem") = [] {
                when("copied to four-slot mem") = [] {
                    my::hijack hj(std::cout);
                    {
                        ch19::mem<int_char> m1(2);
                        m1[0] = {1, 'a'};
                        m1[1] = {2, 'b'};
                        ch19::mem<int_char> m2(4);
                        m2[0] = {3, 'c'};
                        m2[1] = {4, 'd'};
                        m2[2] = {5, 'e'};
                        m2[3] = {6, 'f'};

                        m2 = m1;
                        expect(m2.capacity() == 4_ul);
                        // note that in this case,
                        // capacity never change

                        int sum = 0;
                        for (std::size_t i = 0; i < 2; ++i) {
                            sum += m2[i].n;
                        }
                        expect(sum == 3_i);
                    }
                    expect(hj.release() ==
                        "size 2\n"      // ch19::mem<int_char> m1(2);
                        "size 4\n"      // ch19::mem<int_char> m2(4);
                        "copy 2:4\n"    // copy aissgnment called
                        "del  4\n"      // deleting m2
                        "del  2\n"      // deleting m1
                    );
                };
            };
        };
        "copy constructor"_test = [] {
            given("two-slot mem") = [] {
                when("copy-constructed") = [] {
                    my::hijack hj(std::cout);
                    {
                        ch19::mem<int_char> m1(2);
                        auto m2 {m1};
                        expect(m2.capacity() == 2_ul);
                    }
                    expect(hj.release() ==
                        "size 2\n"
                        "ctor 2\n"
                        "del  2\n"
                        "del  2\n"
                    );
                };
            };
        };
    };

    suite vector_test = [] {
        "default constructor"_test = [] {
            given("a default vector") = [] {
                when("is constructed") = [] {
                    my::hijack hj(std::cout);
                    {
                        ch19::vector<int_char> v;
                        expect(v.size() == 0_ul);
                        expect(v.capacity() == 0_ul);
                    }
                    expect(hj.release() ==
                        "new  0\n"
                        "del  0\n"
                    );
                };
            };
        };
        "explicit constructor"_test = [] {
            given("a vector with three slots") = [] {
                when("gets constructed") = [] {
                    my::hijack hj(std::cout);
                    {
                        ch19::vector<int_char> v(3);
                        expect(v.size() == 3_ul);
                        expect(v.capacity() == 3_ul);
                    }
                    expect(hj.release() ==
                        "size 3\n"
                        "del  3\n"
                    );
                };
            };
        };
        "operator []"_test = [] {
            given("a three-slot vector") = [] {
                when("accessed with []") = [] {
                    my::hijack hj(std::cout);
                    {
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
                    }
                    expect(hj.release() ==
                        "size 3\n"
                        "del  3\n"
                    );
                };
            };
        };
        "a default value"_test = [] {
            given("a vector with three elements") = [] {
                when("constructed with {1, 'a'}") = [] {
                    my::hijack hj(std::cout);
                    {
                        ch19::vector<int_char> v(3, {1, 'a'});
                        expect(v.size() == 3_ul);
                        expect(v.capacity() == 3_ul);

                        int sum = 0;
                        for (std::size_t i = 0; i < v.size(); ++i) {
                            auto [num, ch] = v[i];
                            sum += num;
                        }
                        expect(sum == 3_i);
                    }
                    expect(hj.release() ==
                        "size 3\n"
                        "del  3\n"
                    );
                };
            };
        };
        "initializer"_test = [] {
            given("a vector") = [] {
                when("constructed with a list") = [] {
                    my::hijack hj(std::cout);
                    {
                        ch19::vector<int_char> v {
                            {1, 'a'}, { 2, 'b' }, { 3, 'c' }
                        };
                        int sum = 0;
                        for (std::size_t i = 0; i < v.size(); ++i) {
                            auto [num, ch] = v[i];
                            sum += num;
                        }
                        expect(sum == 6_i);
                    }
                    expect(hj.release() ==
                        "size 3\n"
                        "del  3\n"
                    );
                };
            };
        };
        "reserve"_test = [] {
            given("a vector with three slots") = [] {
                when("reserves two more slots") = [] {
                    my::hijack hj(std::cout);
                    {
                        ch19::vector<int_char> v(3);
                        expect(v.size() == 3_ul);
                        expect(v.capacity() == 3_ul);

                        int sum = 0;
                        for (std::size_t i = 0; i < v.size(); ++i) {
                            auto [num, ch] = v[i];
                            sum += num;
                        }
                        expect(sum == 0_i);

                        v.reserve(v.capacity() + 2);
                        expect(v.size() == 3_ul);
                        expect(v.capacity() == 5_ul);

                        sum = 0;
                        for (std::size_t i = 0; i < v.size(); ++i) {
                            auto [num, ch] = v[i];
                            sum += num;
                        }
                        expect(sum == 0_i);
                    }
                    expect(hj.release() ==
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
                    );
                };
            };
        };
        "copy to shorter"_test = [] {
            given("a four-slot vector") = [] {
                when("copied to two-slot vector") = [] {
                    my::hijack hj(std::cout);
                    {
                        ch19::vector<int_char> v1 {
                            {1, 'a'}, {2, 'b'}, {3, 'c'}, {4, 'd'}
                        };
                        ch19::vector<int_char> v2 {
                            {5, 'e'}, {6, 'f'}
                        };

                        v2 = v1;
                        expect(v2.size() == 4_ul);
                        expect(v2.capacity() == 4_ul);

                        int sum = 0;
                        for (std::size_t i = 0; i < v2.size(); ++i) {
                            auto [num, ch] = v2[i];
                            sum += num;
                        }
                        expect(sum == 10_i);
                    }
                    expect(hj.release() ==
                        "size 4\n"
                        "size 2\n"
                        "copy 4:2\n"
                        "new  4\n"
                        "mtor 4\n"
                        "move 2\n"
                        "move 4\n"
                        "del  0\n"
                        "del  2\n"
                        "del  4\n"
                        "del  4\n"
                    );
                };
            };
        };
        "copy to longer"_test = [] {
            given("a two-slot vector") = [] {
                when("copied to four-slot vector") = [] {
                    my::hijack hj(std::cout);
                    {
                        ch19::vector<int_char> v1 {
                            {1, 'a'}, {2, 'b'}
                        };
                        ch19::vector<int_char> v2 {
                            {3, 'c'}, {4, 'd'}, {5, 'e'}, {6, 'f'}
                        };

                        v2 = v1;
                        expect(v2.size() == 2_ul);
                        expect(v2.capacity() == 4_ul);

                        int sum = 0;
                        for (std::size_t i = 0; i < v2.size(); ++i) {
                            auto [num, ch] = v2[i];
                            sum += num;
                        }
                        expect(sum == 3_i);
                    }
                    expect(hj.release() ==
                        "size 2\n"
                        "size 4\n"
                        "copy 2:4\n"
                        "del  4\n"
                        "del  2\n"
                    );
                };
            };
        };
        "resize longer"_test = [] {
            given("a two-slot vector") = [] {
                when("resizes to four-slot") = [] {
                    my::hijack hj(std::cout);
                    {
                        ch19::vector<int_char> v {
                            {1, 'a'}, {2, 'b'}
                        };

                        v.resize(4);
                        expect(v.size() == 4_ul);
                        expect(v.capacity() == 4_ul);

                        int sum = 0;
                        for (std::size_t i = 0; i < v.size(); ++i) {
                            auto [num, ch] = v[i];
                            sum += num;
                        }
                        expect(sum == 3_i);
                    }
                    expect(hj.release() ==
                        "size 2\n"
                        "resv 2:4\n"
                        "new  4\n"
                        "mtor 4\n"
                        "move 2\n"
                        "move 4\n"
                        "del  0\n"
                        "del  2\n"
                        "del  4\n"
                    );
                };
            };
        };
        "resize longer with a default value"_test = [] {
            given("a two-slot vector") = [] {
                when("resizes to four-slot") = [] {
                    my::hijack hj(std::cout);
                    {
                        ch19::vector<int_char> v {
                            {1, 'a'}, {2, 'b'}
                        };

                        v.resize(4, {3, 'c'});
                        expect(v.size() == 4_ul);
                        expect(v.capacity() == 4_ul);

                        int sum = 0;
                        for (std::size_t i = 0; i < v.size(); ++i) {
                            auto [num, ch] = v[i];
                            sum += num;
                        }
                        expect(sum == 9_i);
                    }
                    expect(hj.release() ==
                        "size 2\n"
                        "resv 2:4\n"
                        "new  4\n"
                        "mtor 4\n"
                        "move 2\n"
                        "move 4\n"
                        "del  0\n"
                        "del  2\n"
                        "del  4\n"
                    );
                };
            };
        };
        "resize to shorter"_test = [] {
            given("a four-slot vector") = [] {
                when("resizes to two-slot") = [] {
                    my::hijack hj(std::cout);
                    {
                        ch19::vector<int_char> v {
                            {1, 'a'}, {2, 'b'}, {3, 'c'}, {4, 'd'}
                        };

                        v.resize(2);
                        expect(v.size() == 2_ul);
                        expect(v.capacity() == 4_ul);

                        int sum = 0;
                        for (std::size_t i = 0; i < v.size(); ++i) {
                            auto [num, ch] = v[i];
                            sum += num;
                        }
                        expect(sum == 3_i);
                    }
                    expect(hj.release() ==
                        "size 4\n"
                        "resv 4:2\n"
                        "del  4\n"
                    );
                };
            };
        };
        "push back"_test = [] {
            given("to an empty vector") = [] {
                when("pushing back one") = [] {
                    my::hijack hj(std::cout);
                    {
                        ch19::vector<int_char> v;

                        int_char val {1, 'a'};
                        v.push_back(val);
                        expect(v.size() == 1_ul);
                        expect(v.capacity() == 8_ul);

                        int sum = 0;
                        for (std::size_t i = 0; i < v.size(); ++i) {
                            auto [num, ch] = v[i];
                            sum += num;
                        }
                        expect(sum == 1_i);
                    }
                    expect(hj.release() ==
                        "new  0\n"
                        "resv 0:8\n"
                        "new  8\n"
                        "mtor 8\n"
                        "move 0\n"
                        "move 8\n"
                        "del  0\n"
                        "del  0\n"
                        "del  8\n"
                    );
                };
            };
            given("to a seven-slot vector") = [] {
                when("pushing back one") = [] {
                    my::hijack hj(std::cout);
                    {
                        ch19::vector<int_char> v {
                            {1, 'a'}, {2, 'b'}, {3, 'c'}, {4, 'd'},
                            {5, 'e'}, {6, 'f'}, {7, 'g'}
                        };

                        int_char val {8, 'h'};
                        v.push_back(val);
                        expect(v.size() == 8_ul);
                        expect(v.capacity() == 14_ul);

                        int sum = 0;
                        for (std::size_t i = 0; i < v.size(); ++i) {
                            auto [num, ch] = v[i];
                            sum += num;
                        }
                        expect(sum == 36_i);
                    }
                    expect(hj.release() ==
                        "size 7\n"
                        "resv 7:14\n"
                        "new  14\n"
                        "mtor 14\n"
                        "move 7\n"
                        "move 14\n"
                        "del  0\n"
                        "del  7\n"
                        "del  14\n"
                    );
                };
            };
            given("to an eight-slot vector") = [] {
                when("pushing back one") = [] {
                    my::hijack hj(std::cout);
                    {
                        ch19::vector<int_char> v {
                            {1, 'a'}, {2, 'b'}, {3, 'c'}, {4, 'd'},
                            {5, 'e'}, {6, 'f'}, {7, 'g'}, {8, 'h'}
                        };

                        int_char val {9, 'i'};
                        v.push_back(val);
                        expect(v.size() == 9_ul);
                        expect(v.capacity() == 16_ul);

                        int sum = 0;
                        for (std::size_t i = 0; i < v.size(); ++i) {
                            auto [num, ch] = v[i];
                            sum += num;
                        }
                        expect(sum == 45_i);
                    }
                    expect(hj.release() ==
                        "size 8\n"
                        "resv 8:16\n"
                        "new  16\n"
                        "mtor 16\n"
                        "move 8\n"
                        "move 16\n"
                        "del  0\n"
                        "del  8\n"
                        "del  16\n"
                    );
                };
            };
            given("an empty slot") = [] {
                when("pushing back ten slots") = [] {
                    my::hijack hj(std::cout);
                    {
                        ch19::vector<int_char> v;

                        for (int i = 0; i < 10; ++i) {
                            int_char val {1 + i, 'a'};
                            v.push_back(val);
                        }

                        expect(v.size() == 10_ul);
                        expect(v.capacity() == 16_ul);

                        int sum = 0;
                        for (std::size_t i = 0; i < v.size(); ++i) {
                            auto [num, ch] = v[i];
                            sum += num;
                        }
                        expect(sum == 55_i);
                    }
                    expect(hj.release() ==
                        "new  0\n"
                        "resv 0:8\n"
                        "new  8\n"
                        "mtor 8\n"
                        "move 0\n"
                        "move 8\n"
                        "del  0\n"
                        "del  0\n"
                        "resv 8:16\n"
                        "new  16\n"
                        "mtor 16\n"
                        "move 8\n"
                        "move 16\n"
                        "del  0\n"
                        "del  8\n"
                        "del  16\n"
                    );
                };
            };
        };
    };
}
