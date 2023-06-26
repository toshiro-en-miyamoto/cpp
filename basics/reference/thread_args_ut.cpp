// g++ thread_args_ut.cpp -std=c++20 -lpthread
#include <vector>
#include <algorithm>
#include <iostream>

void plus(std::vector<int>& v, int d)
{
    std::ranges::for_each(v, [=](int& n) { n += d; });
}

#include <boost/ut.hpp>
#include <my/hijack.h>
#include <thread>
#include <functional>

int main()
{
    using namespace boost::ut;

    "reference"_test = []
    {
        std::vector<int> v {0, 1, 2, 3};
        constexpr int delta = 1;

        my::hijack hj(std::cout);
        plus(v, delta);
        
        for (auto const& i : v) {
            std::cout << i << " ";
        }

        expect(hj.release() ==
            "1 2 3 4 "
        );
    };

    "thread"_test = []
    {
        std::vector<int> v {0, 1, 2, 3};
        constexpr int delta = 1;

        my::hijack hj(std::cout);

        // std::thread t1 {plus, v, delta}; >> compile error
        std::thread t1 {plus, std::ref(v), delta};
        t1.join();
        
        for (auto const& i : v) {
            std::cout << i << " ";
        }

        expect(hj.release() ==
            "1 2 3 4 "
        );
    };

    return 0;
}
