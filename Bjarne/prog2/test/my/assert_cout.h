#pragma once

#include "../boost/ut.hpp"

namespace my {

struct assert_cout
{
    std::ostringstream oss;
    std::streambuf *buf;
    std::string_view text;

#ifdef NDEBUG
    assert_cout(std::string_view text)
    {}
#else
    assert_cout(std::string_view sv)
        : oss {}, buf {std::cout.rdbuf()}, text {sv}
    {
        std::cout.rdbuf(oss.rdbuf());
    }
    ~assert_cout()
    {
        using namespace boost::ut;
        using namespace boost::ut::bdd;
        std::cout.rdbuf(buf);
        expect(oss.str() == text);
    }
#endif
};

}
