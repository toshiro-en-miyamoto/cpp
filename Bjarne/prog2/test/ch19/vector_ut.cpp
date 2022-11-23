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

ch19b::vector<int> make_vec()
{
    return ch19b::vector<int>(6);
}

int main()
{
    using namespace boost::ut;
    using namespace boost::ut::bdd;

    "vector_base"_test = [] {
        ch19b::vector<int> v1;
        ch19b::vector<int> v2(3);
        ch19b::vector<int> v3{3};
        ch19b::vector<int> v4{v2};
        v3 = v2;
        ch19b::vector<int> v5{std::move(make_vec())};
        v5 = std::move(v4);
        v5.reserve(13);
        v5.resize(76);
//        for (auto i = 0; i < 102; ++i) v5.push_back(i);
    };


    "vector_struct"_test = [] {
        ch19b::vector<S> v2{S{1}, S{2}};
        ch19b::vector<S> v3{S{3}, S{4}, S{5}};
        for (auto i = 0; i < v3.size(); ++i) {
            v3[i].id();
        }
        v3 = v2;
        for (auto i = 0; i < v3.size(); ++i) {
            v3[i].id();
        }
    };


    "default constructor"_test = [] {
        given("I have a default vector") = [] {
            ch19u::vector<int> v;
            expect(v.size() == 0_i);
        };
    };

    "vector(size, def)"_test = [] {
        given("I have a vector of 10 elems") = [] {
            ch19u::vector<int> v(10, 1);
            expect(v.size() == 10_i);

            when("accumlating values") = [&] {
                int actual = 0;
                for (auto i = 0; i < 10; ++i) {
                    actual += v[i];
                }
                expect(actual == 10_i);
            };
        };
    };

    "vector(init_list)"_test = [] {
        given("I have a vector with init list") = [] {
            ch19u::vector<int> v{1,2,3,4,5,6,7,8,9,10};
            expect(v.size() == 10_i);

            when("accumlating values") = [&] {
                int actual = 0;
                for (auto i = 0; i < 10; ++i) {
                    actual += v[i];
                }
                expect(actual == 55_i);
            };
        };
    };

    "copy_constructor"_test = [] {
        given("I have a copy of vector") = [] {
            ch19u::vector<int> v1{1,2,3,4,5,6,7,8,9,10};
            ch19u::vector<int> v{v1};
            expect(v.size() == 10_i);

            when("accumlating values") = [&] {
                int actual = 0;
                for (auto i = 0; i < 10; ++i) {
                    actual += v[i];
                }
                expect(actual == 55_i);
            };
        };
    };

    "copy_assignment"_test = [] {
        given("I have a copy of vector") = [] {
            ch19u::vector<int> v1{1,2,3,4,5,6,7,8,9,10};
            ch19u::vector<int> v;
            v = v1;
            expect(v.size() == 10_i);

            when("accumlating values") = [&] {
                int actual = 0;
                for (auto i = 0; i < 10; ++i) {
                    actual += v[i];
                }
                expect(actual == 55_i);
            };
        };
    };

    "move constructor"_test = [] {
        given("I have a copy of vector") = [] {
            ch19u::vector<int> v1{1,2,3,4,5,6,7,8,9,10};
            ch19u::vector<int> v{std::move(v1)};
            expect(v.size() == 10_i);

            when("accumlating values") = [&] {
                int actual = 0;
                for (auto i = 0; i < 10; ++i) {
                    actual += v[i];
                }
                expect(actual == 55_i);
            };
        };
    };

    "move assignment"_test = [] {
        given("I have a copy of vector") = [] {
            ch19u::vector<int> v1{1,2,3,4,5,6,7,8,9,10};
            ch19u::vector<int> v;
            v = std::move(v1);
            expect(v.size() == 10_i);

            when("accumlating values") = [&] {
                int actual = 0;
                for (auto i = 0; i < 10; ++i) {
                    actual += v[i];
                }
                expect(actual == 55_i);
            };
        };
    };

    "reserve"_test = [] {
        given("I have a copy of vector") = [] {
            ch19u::vector<int> v{1,2,3,4,5,6,7,8,9,10};
            expect(v.size() == 10_i);

            when("reserve for 20 elements") = [&] {
                v.reserve(20);
                expect(v.size() == 10_i);
                expect(v.capacity() == 20_i);
            };
        };
    };

    "resize"_test = [] {
        given("I have a copy of vector") = [] {
            ch19u::vector<int> v{1,2,3,4,5,6,7,8,9,10};
            expect(v.size() == 10_i);

            when("resize to 12 elements with 1") = [&] {
                v.resize(12, 1);
                expect(v.size() == 12_i);
                expect(v[10] == 1_i);
                expect(v[11] == 1_i);
            };
        };
    };

    "push_back"_test = [] {
        given("I have a copy of vector") = [] {
            ch19u::vector<int> v{1,2,3,4,5,6,7,8,9,10};
            expect(v.size() == 10_i);

            when("push back 11") = [&] {
                v.push_back(11);
                expect(v.size() == 11_i);
                expect(v[10] == 11_i);
                expect(v.capacity() == 20_i);
            };
        };
    };

}
