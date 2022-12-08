#include <typeinfo>
#include <boost/ut.hpp>

// these five template functions help cheet how our compiler
// deduces the types of 'T' and 'arg'
using type_inf = const std::type_info&;

template<typename T>
bool f_val(T arg, type_inf type_t, type_inf arg_t
) { return typeid(T) == type_t && typeid(arg) == arg_t; }

template<typename T>
bool f_ptr(T* arg, type_inf type_t, type_inf arg_t
) { return typeid(T) == type_t && typeid(arg) == arg_t; }

template<typename T>
bool f_ref(T& arg, type_inf type_t, type_inf arg_t
) { return typeid(T) == type_t && typeid(arg) == arg_t; }

template<typename T>
bool f_conref(const T& arg, type_inf type_t, type_inf arg_t
) { return typeid(T) == type_t && typeid(arg) == arg_t; }

template<typename T>
bool f_refref(T&& arg, type_inf type_t, type_inf arg_t
) { return typeid(T) == type_t && typeid(arg) == arg_t; }

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
                using func_ptr_t = void (*)(int, char);
                expect(f_val(func, typeid(func_ptr_t), typeid(func_ptr_t)));
            };
            when("calling f() taking T&") = [] {
                using func_ref_t = void (&)(int, char);
                expect(f_ref(func, typeid(func_ref_t), typeid(func_ref_t)));
            };
        };
    };
    "arrays decay to pointers"_test = [] {
        given("array argument") = [] {
            when("calling f() taking T") = [] {
                const char name[] = "Briggs";

                using ch_ptr_t = const char*;
                expect(f_val(name, typeid(ch_ptr_t), typeid(ch_ptr_t)));
            };
            when("calling f() taking T&") = [] {
                const char name[] = "Briggs";

                using arr_t = const char[7];
                using arr_ref_t = const char (&)[7];
                expect(f_ref(name, typeid(arr_t), typeid(arr_ref_t)));
            };
        };
    };

    "case3: pass-by-value"_test = [] {
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

    "case2: pass by universal ref"_test = [] {
        given("non-const argument") = [] {
            when("calling f() taking T&&") = [] {
                int arg = 27;

                using ref_t = int&;
                expect(f_refref(arg, typeid(ref_t), typeid(ref_t)));
            };
        };
        given("const argument") = [] {
            when("calling f() taking T&&") = [] {
                const int arg = 27;

                using ref_t = const int&;
                expect(f_refref(arg, typeid(ref_t), typeid(ref_t)));
            };
        };
        given("const ref argument") = [] {
            when("calling f() taking T&&") = [] {
                int x = 27;
                const int& arg = x;

                using ref_t = const int&;
                expect(f_refref(arg, typeid(ref_t), typeid(ref_t)));
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
                int x = 27;
                int *arg = &x;

                using val_t = int;
                using ptr_t = int*;
                expect(f_ptr(arg, typeid(val_t), typeid(ptr_t)));
            };
        };
        given("const pointer argument") = [] {
            when("calling f() taking T*") = [] {
                int x = 27;
                const int *arg = &x;

                using val_t = const int;
                using ptr_t = const int*;
                expect(f_ptr(arg, typeid(val_t), typeid(ptr_t)));
            };
        };
    };

    "case1b"_test = [] {
        given("non-const argument") = [] {
            when("calling f() taking const T&") = [] {
                int arg = 27;

                using val_t = int;
                using ref_t = const int&;
                expect(f_conref(arg, typeid(val_t), typeid(ref_t)));
            };
        };
        given("const argument") = [] {
            when("calling f() taking const T&") = [] {
                const int arg = 27;

                using val_t = int;
                using ref_t = const int&;
                expect(f_conref(arg, typeid(val_t), typeid(ref_t)));
            };
        };
        given("const ref argument") = [] {
            when("calling f() taking const T&") = [] {
                int x = 27;
                const int& arg = x;

                using val_t = int;
                using ref_t = const int&;
                expect(f_conref(arg, typeid(val_t), typeid(ref_t)));
            };
        };
    };

    "case1a"_test = [] {
        given("non-const argument") = [&] {
            when("calling f() taking T&") = [&] {
                int arg = 27;

                using val_t = int;
                using ref_t = int&;
                expect(f_ref(arg, typeid(val_t), typeid(ref_t)));
            };
        };
        given("const argument") = [] {
            when("calling f() taking T&") = [] {
                const int arg = 27;

                using val_t = const int;
                using ref_t = const int&;
                expect(f_ref(arg, typeid(val_t), typeid(ref_t)));
            };
        };
        given("const ref argument") = [] {
            when("calling f() taking T&") = [] {
                int x = 27;
                const int& arg = x;

                using val_t = const int;
                using ref_t = const int&;
                expect(f_ref(arg, typeid(val_t), typeid(ref_t)));
            };
        };
    };
}
