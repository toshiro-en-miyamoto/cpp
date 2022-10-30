#include <ch17/typesize.h>
#include <boost/ut.hpp>

int main()
{
    using namespace boost::ut;

    "size of char"_test = [] {
        auto [type_size, value_size] = my::typesize<char>('a');
        expect(type_size == 1_i);
        expect(value_size == 1_i);
    };
    "size of int"_test = [] {
        auto [type_size, value_size] = my::typesize<int>(1);
        expect(type_size == 4_i);
        expect(value_size == 4_i);
    };
    "size of long"_test = [] {
        auto [type_size, value_size] = my::typesize<long>(1L);
        expect(type_size == 8_i);
        expect(value_size == 8_i);
    };
    "size of pointer to int"_test = [] {
        int i = 1;
        int* pi = &i;
        auto [type_size, value_size] = my::typesize<int*>(pi);
        expect(type_size == 8_i);
        expect(value_size == 8_i);
    };
    "size of bool"_test = [] {
        auto [type_size, value_size] = my::typesize<bool>(true);
        expect(type_size == 1_i);
        expect(value_size == 1_i);
    };
    "size of double"_test = [] {
        auto [type_size, value_size] = my::typesize<double>(1);
        expect(type_size == 8_i);
        expect(value_size == 8_i);
    };
}
