#include <ch18/drill.h>
#include <boost/ut.hpp>
#include <iostream>
#include <sstream>
#include <vector>

int main()
{
    using namespace boost::ut;

    "array drill 1"_test = [] () {
        std::ostringstream test_out;
        auto cout_buff = std::cout.rdbuf();
        std::cout.rdbuf(test_out.rdbuf());

        my::fa(my::ga, 10);                     // array 04.a

        std::cout.rdbuf(cout_buff);
        expect(
            test_out.str() ==
            "1;2;4;8;16;32;64;128;256;512;"
            "1;2;4;8;16;32;64;128;256;512;"
        ) << test_out.str();
    };

    "array drill 2"_test = [] {
        std::ostringstream test_out;
        auto cout_buff = std::cout.rdbuf();
        std::cout.rdbuf(test_out.rdbuf());

        int aa[10]                              // array 04.b
        {1,2,6,24,120,720,5040,40320,362880,3628800};

        my::fa(aa, 10);                         // array 04.c

        std::cout.rdbuf(cout_buff);
        expect(
            test_out.str() ==
            "1;2;4;8;16;32;64;128;256;512;"
            "1;2;6;24;120;720;5040;40320;362880;3628800;"
        ) << test_out.str();
    };

    "vector drill 1"_test = [] {
        std::ostringstream test_out;
        auto cout_buff = std::cout.rdbuf();
        std::cout.rdbuf(test_out.rdbuf());

        my::fv(my::gv);                         // vector 04.a

        std::cout.rdbuf(cout_buff);
        expect(
            test_out.str() ==
            "1;2;4;8;16;32;64;128;256;512;"
            "1;2;4;8;16;32;64;128;256;512;"
        ) << test_out.str();
    };

    "vector drill 2"_test = [] {
        std::ostringstream test_out;
        auto cout_buff = std::cout.rdbuf();
        std::cout.rdbuf(test_out.rdbuf());

        std::vector<int>                        // vector 04.b
        vv {1,2,6,24,120,720,5040,40320,362880,3628800};

        my::fv(vv);                             // vector 04.c

        std::cout.rdbuf(cout_buff);
        expect(
            test_out.str() ==
            "1;2;4;8;16;32;64;128;256;512;"
            "1;2;6;24;120;720;5040;40320;362880;3628800;"
        ) << test_out.str();
    };
}
