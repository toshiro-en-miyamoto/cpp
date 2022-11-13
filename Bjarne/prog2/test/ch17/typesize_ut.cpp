#include <ch17/typesize.h>
#include <boost/ut.hpp>

int main()
{
    using namespace boost::ut;

    suite typesize = []
    {
        "size of char"_test = []
        {
            auto [type_size, value_size] = ch17::typesize<char>('a');
            expect(type_size == 1_i);
            expect(value_size == 1_i);
        };
        "size of int"_test = []
        {
            auto [type_size, value_size] = ch17::typesize<int>(1);
            expect(type_size == 4_i);
            expect(value_size == 4_i);
        };
        "size of long"_test = []
        {
            auto [type_size, value_size] = ch17::typesize<long>(1L);
            expect(type_size == 8_i);
            expect(value_size == 8_i);
        };
        "size of pointer to int"_test = []
        {
            int i = 1;
            int *pi = &i;
            auto [type_size, value_size] = ch17::typesize<int *>(pi);
            expect(type_size == 8_i);
            expect(value_size == 8_i);
        };
        "size of bool"_test = []
        {
            auto [type_size, value_size] = ch17::typesize<bool>(true);
            expect(type_size == 1_i);
            expect(value_size == 1_i);
        };
        "size of double"_test = []
        {
            auto [type_size, value_size] = ch17::typesize<double>(1);
            expect(type_size == 8_i);
            expect(value_size == 8_i);
        };
    };

    suite typesize2 = []
    {
        "size of char"_test = []
        {
            auto type_size = ch17::typesize2('a');
            expect(type_size == 1_i);
        };
        "size of int"_test = []
        {
            auto type_size = ch17::typesize2(1);
            expect(type_size == 4_i);
        };
        "size of long"_test = []
        {
            auto type_size = ch17::typesize2(1L);
            expect(type_size == 8_i);
        };
        "size of pointer to int"_test = []
        {
            int i = 1;
            int *pi = &i;
            auto type_size = ch17::typesize2(pi);
            expect(type_size == 8_i);
        };
        "size of bool"_test = []
        {
            auto type_size = ch17::typesize2(true);
            expect(type_size == 1_i);
        };
        "size of double"_test = []
        {
            auto type_size = ch17::typesize2<double>(1);
            expect(type_size == 8_i);
        };
    };
}
