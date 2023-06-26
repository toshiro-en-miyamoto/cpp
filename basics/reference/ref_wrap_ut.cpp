#include <list>
#include <numeric>
#include <vector>
#include <functional>
#include <algorithm>
#include <iostream>
#include <boost/ut.hpp>
#include <my/hijack.h>

int main()
{
    using namespace boost::ut;
    using namespace boost::ut::bdd;

    "reference_wrapper"_test = []
    {
        given("a list") = []
        {
            std::list<int> l(9);
            std::iota(l.begin(), l.end(), -4);
            std::vector<std::reference_wrapper<int>> v {l.begin(), l.end()};

            when("before reversing") = [&l, &v]
            {
                my::hijack hj(std::cout);

                std::cout << "list  : ";
                for (auto const& i : l) {
                    std::cout << i << " ";
                }
                std::cout << std::endl;

                std::cout << "vector: ";
                for (auto const& i : v) {
                    std::cout << i << " ";
                }
                std::cout << std::endl;

                expect(hj.release() ==
                    "list  : -4 -3 -2 -1 0 1 2 3 4 \n"
                    "vector: -4 -3 -2 -1 0 1 2 3 4 \n"
                );
            };
            when("after reversing") = [&l, &v]
            {
                std::reverse(v.begin(), v.end());

                my::hijack hj(std::cout);

                std::cout << "list  : ";
                for (auto const& i : l) {
                    std::cout << i << " ";
                }
                std::cout << std::endl;

                std::cout << "vector: ";
                for (auto const& i : v) {
                    std::cout << i << " ";
                }
                std::cout << std::endl;

                expect(hj.release() ==
                    "list  : -4 -3 -2 -1 0 1 2 3 4 \n"
                    "vector: 4 3 2 1 0 -1 -2 -3 -4 \n"
                );
            };
        };
    };
}
