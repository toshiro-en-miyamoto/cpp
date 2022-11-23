#include <ch19/vector.h>
#include <boost/ut.hpp>
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

ch19::vector<int> make_vec()
{
    return ch19::vector<int>(6);
}

int main()
{
    using namespace boost::ut;

"vector_base"_test = [] {
    ch19::vector<int> v1;
    ch19::vector<int> v2(3);
    ch19::vector<int> v3{3};
    ch19::vector<int> v4{v2};
    v3 = v2;
    ch19::vector<int> v5{std::move(make_vec())};
    v5 = std::move(v4);
    v5.reserve(13);
    v5.resize(76);
    for (auto i = 0; i < 102; ++i) v5.push_back(i);
};

"vector_struct"_test = [] {
    ch19::vector<S> v2{S{1}, S{2}};
    ch19::vector<S> v3{S{3}, S{4}, S{5}};
    for (auto i = 0; i < v3.size(); ++i) {
        v3[i].id();
    }
    v3 = v2;
    for (auto i = 0; i < v3.size(); ++i) {
        v3[i].id();
    }

};

}
