#include <ch26/binary_search.h>
#include <boost/ut.hpp>

int main()
{
    using namespace boost::ut;
    using namespace boost::ut::bdd;

    "binary_search"_test = [] {
        given("odd number of elements") = [] {
            const char data[] = "cdefghijklmno";
            when("one before the first") = [&] {
                expect(my::binary_search(data, 13, 'b') == 13_ul);
            };
            when("the first") = [&] {
                expect(my::binary_search(data, 13, 'c') == 0_ul);
            };
            when("one after the first") = [&] {
                expect(my::binary_search(data, 13, 'd') == 1_ul);
            };
            when("one before the middle") = [&] {
                expect(my::binary_search(data, 13, 'h') == 5_ul);
            };
            when("the middel") = [&] {
                expect(my::binary_search(data, 13, 'i') == 6_ul);
            };
            when("one after the middle") = [&] {
                expect(my::binary_search(data, 13, 'j') == 7_ul);
            };
            when("one before the last") = [&] {
                expect(my::binary_search(data, 13, 'n') == 11_ul);
            };
            when("the last") = [&] {
                expect(my::binary_search(data, 13, 'o') == 12_ul);
            };
            when("one after the last") = [&] {
                expect(my::binary_search(data, 13, 'p') == 13_ul);
            };
        };
        given("even number of elements") = [] {
            const char data[] = "cdefghijklmnop";
            when("one before the first") = [&] {
                expect(my::binary_search(data, 14, 'b') == 14_ul);
            };
            when("the first") = [&] {
                expect(my::binary_search(data, 14, 'c') == 0_ul);
            };
            when("one after the first") = [&] {
                expect(my::binary_search(data, 14, 'd') == 1_ul);
            };
            when("one before the left-middle") = [&] {
                expect(my::binary_search(data, 14, 'h') == 5_ul);
            };
            when("the left-middel") = [&] {
                expect(my::binary_search(data, 14, 'i') == 6_ul);
            };
            when("the right-middle") = [&] {
                expect(my::binary_search(data, 14, 'j') == 7_ul);
            };
            when("one after the right-middle") = [&] {
                expect(my::binary_search(data, 14, 'k') == 8_ul);
            };
            when("one before the last") = [&] {
                expect(my::binary_search(data, 14, 'o') == 12_ul);
            };
            when("the last") = [&] {
                expect(my::binary_search(data, 14, 'p') == 13_ul);
            };
            when("one after the last") = [&] {
                expect(my::binary_search(data, 14, 'q') == 14_ul);
            };
        };
    };
}
