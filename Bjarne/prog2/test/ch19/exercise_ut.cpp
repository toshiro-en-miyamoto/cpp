#include <ch19/exercise.h>
#include <boost/ut.hpp>

int main()
{
    using namespace boost::ut;
    using namespace boost::ut::bdd;

    "ex 01"_test = [] {
        given("three-element vector<int>") = [] {
            const std::vector<int> cv {1, 2, 3};
            when("added to a shorter vector") = [&] {
                std::vector<int> v {3, 4};
                ch19::add(v, cv);
                expect(v[0] == 4_i);
                expect(v[1] == 6_i);
            };
            when("added to a longer vector") = [&] {
                std::vector<int> v {3, 4, 5, 6};
                ch19::add(v, cv);
                expect(v[0] == 4_i);
                expect(v[1] == 6_i);
                expect(v[2] == 8_i);
                expect(v[3] == 6_i);
            };
        };
    };
    "ex 02"_test = [] {
        given("vectors of int and double") = [] {
            const std::vector<int> iv {1, 2, 3};
            const std::vector<double> dv {2.0, 3.0, 4.0};
            expect(ch19::product(iv, dv) == 20.0_d);
        };
    };
}
