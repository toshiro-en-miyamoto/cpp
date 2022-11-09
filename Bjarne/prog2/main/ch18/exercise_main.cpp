#include <ch18/exercise.h>
#include <string_view>
#include <cassert>

void exercise2()
{
    const char p[] = "exercise";
    const char x1[] = "xe";
    auto found = my::findx(p, x1);
    assert(found == p+1);
}

void exercise1()
{
    constexpr char pi[] = "exercise";
    const std::string_view svi{pi};

    char* po = my::strdup(pi);
    std::string_view svo{po};
    assert(svi == svo);
    delete[] po;
}

int main()
{
    exercise2();
    exercise1();
}
