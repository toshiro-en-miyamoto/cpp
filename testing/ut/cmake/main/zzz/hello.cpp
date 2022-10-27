#include "hello.h"

namespace my {

std::string greeting(const std::string& who)
{
    const std::string greeting = "Hello " + who;
    return greeting;
}

} // end of namespace
