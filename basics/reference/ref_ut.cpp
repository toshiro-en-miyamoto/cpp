#include <string>
#include <boost/ut.hpp>
#include <my/hijack.h>
#include <iostream>

int main()
{
    using namespace boost::ut;

    "reference"_test = []
    {
        int v[] = {1, 2, 3, 4};
        for (auto& x : v) ++x;

        my::hijack hj(std::cout);
        for (auto const& i : v) std::cout << i << " ";
        expect(hj.release() == "2 3 4 5 ");
    };

    "lvalue reference"_test = []
    {
        auto char_at = []
        (std::string& s, std::string::size_type pos) -> char&
        {
            return s[pos];
        };

        std::string s = "Test";
        char_at(s, 0) = 'R';
        expect(s == "Rest");
    };
}
