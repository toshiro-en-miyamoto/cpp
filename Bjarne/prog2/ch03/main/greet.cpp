#include "greet.h"
#include <string>

void greet(std::istream& c_in, std::ostream& c_out)
{
    std::string first_name;
    int age;
    c_in >> first_name;
    c_in >> age;
    c_out << "Hello, " << first_name << " (" << age << ')';
}
