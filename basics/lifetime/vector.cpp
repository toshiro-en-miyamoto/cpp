#include "vector.h"
#include "../include/boost/ut.hpp"

int main()
{
    using namespace boost::ut;
    using namespace boost::ut::bdd;

    "default constructor"_test = [] {
        given("a default vector") = [] {
            vector<int> v;
            when("checking its size") = [&] {
                expect(v.size() == 0_ul);
                expect(v.capacity() == 0_ul);
            };
        };
    };

    "sized constructor"_test = [] {
        given("three 2 in a vector") = [] {
            vector<int> v(2, 3);
            expect(v.size() == 2_ul);
            expect(v.capacity() == 2_ul);
            when("accumulating") = [&] {
                int actual = 0;
                for (auto i = 0; i < v.size(); ++i)
                    actual += v[i];
                expect(actual == 6_i);
            };
        };
    };

    "init_list constructor"_test = [] {
        given("1 2 3 4 in a vector") = [] {
            vector<int> v{1, 2, 3, 4};
            expect(v.size() == 4_ul);
            expect(v.capacity() == 4_ul);
            when("accumulating") = [&] {
                int actual = 0;
                for (auto i = 0; i < v.size(); ++i)
                    actual += v[i];
                expect(actual == 10_i);
            };
        };
    };
}
