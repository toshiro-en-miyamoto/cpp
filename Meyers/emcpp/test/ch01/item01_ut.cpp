#include <typeinfo>
#include <utility>
#include <boost/ut.hpp>

// these five template functions help cheet how our compiler
// deduces the types of 'T' and 'param'
using type_ref = const std::type_info&;
using type_pair = std::pair<type_ref, type_ref>;

template<typename T> type_pair ff_val(T param)
{ return {typeid(T), typeid(param)}; }

template<typename T> type_pair ff_ptr(T* param)
{ return {typeid(T), typeid(param)}; }

template<typename T> type_pair ff_ref(T& param)
{ return {typeid(T), typeid(param)}; }

template<typename T> type_pair ff_conref(const T& param)
{ return {typeid(T), typeid(param)}; }

template<typename T> type_pair ff_refref(T&& param)
{ return {typeid(T), typeid(param)}; }

// for "functions decay to pointers"_test
void func(int, char)
{}

int main()
{
    using namespace boost::ut;
    using namespace boost::ut::bdd;

    "functions decay to pointers"_test = [] {
        given("a function") = [] {
            when("calling f() taking T") = [] {
                auto [type_arg, param_arg] = ff_val(func);

                using func_ptr_t = void (*)(int, char);
                expect(type_arg  == typeid(func_ptr_t));
                expect(param_arg == typeid(func_ptr_t));
            };
            when("calling f() taking T&") = [] {
                auto [type_arg, param_arg] = ff_ref(func);

                using func_ref_t = void (&)(int, char);
                expect(type_arg  == typeid(func_ref_t));
                expect(param_arg == typeid(func_ref_t));
            };
        };
    };
    "arrays decay to pointers"_test = [] {
        given("array argument") = [] {
            when("calling f() taking T") = [] {
                const char name[] = "Briggs";
                auto [type_arg, param_arg] = ff_val(name);

                using ch_ptr_t = const char*;
                expect(type_arg  == typeid(ch_ptr_t));
                expect(param_arg == typeid(ch_ptr_t));
            };
            when("calling f() taking T&") = [] {
                const char name[] = "Briggs";
                auto [type_arg, param_arg] = ff_ref(name);

                using arr_t = const char[7];
                using arr_ref_t = const char (&)[7];
                expect(type_arg  == typeid(arr_t));
                expect(param_arg == typeid(arr_ref_t));
            };
        };
    };

    "case3: pass-by-value"_test = [] {
        given("non-const argument") = [] {
            when("calling f() taking T") = [] {
                int x = 27;
                auto [type_arg, param_arg] = ff_val(x);

                expect(type_arg  == typeid(int));
                expect(param_arg == typeid(int));
            };
        };
        given("const argument") = [] {
            when("calling f() taking T") = [] {
                int x = 27;
                const int cx = x;
                auto [type_arg, param_arg] = ff_val(cx);

                expect(type_arg  == typeid(int));
                expect(param_arg == typeid(int));
            };
        };
        given("const ref argument") = [] {
            when("calling f() taking T") = [] {
                int x = 27;
                const int& rx = x;
                auto [type_arg, param_arg] = ff_val(rx);

                expect(type_arg  == typeid(int));
                expect(param_arg == typeid(int));
            };
        };
    };

    "case2: pass by universal ref"_test = [] {
        given("non-const argument") = [] {
            when("calling f() taking T&&") = [] {
                int x = 27;
                auto [type_arg, param_arg] = ff_refref(x);

                expect(type_arg  == typeid(int&));
                expect(param_arg == typeid(int&));
            };
        };
        given("const argument") = [] {
            when("calling f() taking T&&") = [] {
                int x = 27;
                const int cx = x;
                auto [type_arg, param_arg] = ff_refref(cx);

                expect(type_arg  == typeid(const int&));
                expect(param_arg == typeid(const int&));
            };
        };
        given("const ref argument") = [] {
            when("calling f() taking T&&") = [] {
                int x = 27;
                const int& rx = x;
                auto [type_arg, param_arg] = ff_refref(rx);

                expect(type_arg  == typeid(const int&));
                expect(param_arg == typeid(const int&));
            };
        };
        given("literal argument") = [] {
            when("calling f() taking T&&") = [] {
                auto [type_arg, param_arg] = ff_refref(27);

                expect(type_arg  == typeid(const int));
                expect(param_arg == typeid(const int&&));
            };
        };
    };

    "case1c"_test = [] {
        given("non-const argument") = [] {
            when("calling f() taking T*") = [] {
                int x = 27;
                int *px = &x;
                auto [type_arg, param_arg] = ff_ptr(px);

                expect(type_arg  == typeid(int));
                expect(param_arg == typeid(int*));
            };
        };
        given("const pointer argument") = [] {
            when("calling f() taking T*") = [] {
                int x = 27;
                const int *cpx = &x;
                auto [type_arg, param_arg] = ff_ptr(cpx);

                expect(type_arg  == typeid(const int));
                expect(param_arg == typeid(const int*));
            };
        };
    };

    "case1b"_test = [] {
        given("non-const argument") = [] {
            when("calling f() taking const T&") = [] {
                int x = 27;
                auto [type_arg, param_arg] = ff_conref(x);

                expect(type_arg  == typeid(int));
                expect(param_arg == typeid(const int&));
            };
        };
        given("const argument") = [] {
            when("calling f() taking const T&") = [] {
                int x = 27;
                const int cx = x;
                auto [type_arg, param_arg] = ff_conref(cx);

                expect(type_arg  == typeid(int));
                expect(param_arg == typeid(const int&));
            };
        };
        given("const ref argument") = [] {
            when("calling f() taking const T&") = [] {
                int x = 27;
                const int& rx = x;
                auto [type_arg, param_arg] = ff_conref(rx);

                expect(type_arg  == typeid(int));
                expect(param_arg == typeid(const int&));
            };
        };
    };

    "case1a"_test = [] {
        given("non-const argument") = [&] {
            when("calling f() taking T&") = [&] {
                int x = 27;
                auto [type_arg, param_arg] = ff_ref(x);

                expect(type_arg  == typeid(int));
                expect(param_arg == typeid(int&));
            };
        };
        given("const argument") = [] {
            when("calling f() taking T&") = [] {
                int x = 27;
                const int cx = x;
                auto [type_arg, param_arg] = ff_ref(cx);

                expect(type_arg  == typeid(const int));
                expect(param_arg == typeid(const int&));
            };
        };
        given("const ref argument") = [] {
            when("calling f() taking T&") = [] {
                int x = 27;
                const int& rx = x;
                auto [type_arg, param_arg] = ff_ref(rx);

                expect(type_arg  == typeid(const int));
                expect(param_arg == typeid(const int&));
            };
        };
    };
}
