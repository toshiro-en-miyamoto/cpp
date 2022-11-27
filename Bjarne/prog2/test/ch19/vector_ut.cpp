#include <ch19/vector.h>
#include <boost/ut.hpp>
#include <iostream>
#include <sstream>

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
                    "del  0\n")
                << local.str();
            };
        };
        "reserve"_test = [] {
            given("a default mem") = [] {
                std::ostringstream local;
                auto cout_buff = std::cout.rdbuf();
                std::cout.rdbuf(local.rdbuf());

                when("reserves three elememts") = [] {
                    ch19::mem<int_char> m;
                    m.reserve(3);
                    expect(m.elem() != nullptr);
                    expect(m.capacity() == 3_ul);
                };

                std::cout.rdbuf(cout_buff);
                expect(local.str() ==
                    "new  0\n"
                    "reserving from 0 to 3\n"
                    "new  3\n"  // mem new_mem {new_p, 3}
                                // swap(new_mem, *this)
                    "mtor 3\n"  //      tmp {move(new_mem)}
                    "move 0\n"  //      new_mem = move(*this)
                    "move 3\n"  //      *this = move(tmp)
                    "del  0\n"  //      deleting tmp
                    "del  0\n"  // deleting new_mem (formally *this)
                    "reserved\n"
                    "del  3\n") // deleting m (oramlly new_mem)
                << local.str();
            };
        };
    };

    suite vector_test = [] {};
}
