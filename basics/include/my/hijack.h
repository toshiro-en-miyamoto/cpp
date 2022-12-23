#pragma once

#include <ostream>
#include <sstream>

namespace my {

/**
 * Borrows std::streambuf of std::ostream until released or destructed.
 * release() returns a moveable std::string.
 *
 * sample usage:
    my::hijack hj(std::cout);
    std::cout << "Hello";
    std::cout << hj.release().append(" world!\n");
    // prints "Hello world!\n" to std::cout
 */
class hijack
{
    std::ostream& os_;
    std::streambuf* buf;
    std::ostringstream local;
public:
    explicit hijack(std::ostream& os)
        : os_ {os}, buf {os.rdbuf()}, local {}
    {
        os.rdbuf(local.rdbuf());
    }

    ~hijack()
    {
        if (buf) os_.rdbuf(buf);
    }

    std::string release()
    {
        os_.rdbuf(buf);
        buf = nullptr;
        return local.str();
    }
};

}
