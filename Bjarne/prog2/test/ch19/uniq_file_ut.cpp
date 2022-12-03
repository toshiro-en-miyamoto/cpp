#include <ch19/fille_handle.h>
#include <iostream>

int main(int argc, char* argv[])
{
    // argv[1] : file name
    if (!argv[1]) exit(1);
    ch19::file_handle_t fp {argv[1], "r"};
    auto file = fp.get();
    for (int c; (c = std::fgetc(file)) != EOF;) {
        std::putchar(c);
    }
}
