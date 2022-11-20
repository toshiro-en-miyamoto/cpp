#include <ch19/vector.h>
#include <iostream>

class S {
    inline static int n{1};
    int m{};
    void pre() const { std::cout << "#" << m << std::string(m, ' '); }
public:
    S(int x) : m{n++} { pre(); std::cout << "S::S(" << x << ");\n"; }
    ~S() { pre(); std::cout << "S::~S();\n"; }
    void id() const { pre(); std::cout << "S::id();\n"; }
};

int main()
{
    ch19::vector<S> v1{S{1}};
    ch19::vector<S> v3{S{1}, S{2}, S{3}};
    for (auto i = 0; i < v3.size(); ++i) {
        v3[i].id();
    }
    v3 = v1;
    for (auto i = 0; i < v3.size(); ++i) {
        v3[i].id();
    }
}
