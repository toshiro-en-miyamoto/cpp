#include <typeinfo>
#include <boost/ut.hpp>

using namespace boost::ut;
using namespace boost::ut::bdd;

template<typename T>
bool f_val(T arg, const std::type_info& type_t, const std::type_info& arg_t
) { return typeid(T) == type_t && typeid(arg) == arg_t; }

template<typename T>
bool f_refref(T&& arg, const std::type_info& type_t, const std::type_info& arg_t
) { return typeid(T) == type_t && typeid(arg) == arg_t; }

template<typename T>
bool f_const_ref(const T& arg, const std::type_info& type_t, const std::type_info& arg_t
) { return typeid(T) == type_t && typeid(arg) == arg_t; }

template<typename T>
bool f_ref(T& arg, const std::type_info& type_t, const std::type_info& arg_t
) { return typeid(T) == type_t && typeid(arg) == arg_t; }

template<typename T>
bool f_ptr(T* arg, const std::type_info& type_t, const std::type_info& arg_t
) { return typeid(T) == type_t && typeid(arg) == arg_t; }

int main()
{
    "case4"_test = [] {
        given("array argument") = [] {
            when("calling f() taking T&") = [] {
                const char name[] = "Briggs";
                expect(f_ref(name, typeid(const char[7]), typeid(const char (&)[7])));
            };
            when("calling f() taking T") = [] {
                const char name[] = "Briggs";
                expect(f_val(name, typeid(const char*), typeid(const char*)));
            };
        };
    };

    "case3"_test = [] {
        given("non-const argument") = [] {
            when("calling f() taking T") = [] {
                int arg = 27;
                expect(f_val(arg, typeid(int), typeid(int)));
            };
        };
        given("const argument") = [] {
            when("calling f() taking T") = [] {
                const int arg = 27;
                expect(f_val(arg, typeid(int), typeid(int)));
            };
        };
        given("const ref argument") = [] {
            when("calling f() taking T") = [] {
                int x = 27;
                const int& arg = x;
                expect(f_val(arg, typeid(int), typeid(int)));
            };
        };
    };

    "case2"_test = [] {
        given("non-const argument") = [] {
            when("calling f() taking T&&") = [] {
                int arg = 27;
                expect(f_refref(arg, typeid(int&), typeid(int&)));
            };
        };
        given("const argument") = [] {
            when("calling f() taking T&&") = [] {
                const int arg = 27;
                expect(f_refref(arg, typeid(const int&), typeid(const int&)));
            };
        };
        given("const ref argument") = [] {
            when("calling f() taking T&&") = [] {
                int x = 27;
                const int& arg = x;
                expect(f_refref(arg, typeid(const int&), typeid(const int&)));
            };
        };
        given("literal argument") = [] {
            when("calling f() taking T&&") = [] {
                expect(f_refref(27, typeid(int), typeid(int&&)));
            };
        };
    };

    "case1c"_test = [] {
        given("non-const argument") = [] {
            when("calling f() taking T*") = [] {
                int arg = 27;
                expect(f_ptr(&arg, typeid(int), typeid(int*)));
            };
        };
        given("const pointer argument") = [] {
            when("calling f() taking T*") = [] {
                int x = 27;
                const int *arg = &x;
                expect(f_ptr(arg, typeid(const int), typeid(const int*)));
            };
        };
    };

    "case1b"_test = [] {
        given("non-const argument") = [] {
            when("calling f() taking const T&") = [] {
                int arg = 27;
                expect(f_const_ref(arg, typeid(int), typeid(const int&)));
            };
        };
        given("const argument") = [] {
            when("calling f() taking const T&") = [] {
                const int arg = 27;
                expect(f_const_ref(arg, typeid(int), typeid(const int&)));
            };
        };
        given("const ref argument") = [] {
            when("calling f() taking const T&") = [] {
                int x = 27;
                const int& arg = x;
                expect(f_const_ref(arg, typeid(int), typeid(const int&)));
            };
        };
    };

    "case1a"_test = [] {
        given("non-const argument") = [&] {
            when("calling f() taking T&") = [&] {
                int arg = 27;
                expect(f_ref(arg, typeid(int), typeid(int&)));
            };
        };
        given("const argument") = [] {
            when("calling f() taking T&") = [] {
                const int arg = 27;
                expect(f_ref(arg, typeid(const int), typeid(const int&)));
            };
        };
        given("const ref argument") = [] {
            when("calling f() taking T&") = [] {
                int x = 27;
                const int& arg = x;
                expect(f_ref(arg, typeid(const int), typeid(const int&)));
            };
        };
    };
}
