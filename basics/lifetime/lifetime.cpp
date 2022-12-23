#include <iostream>
#include <boost/ut.hpp>
#include <my/hijack.h>

struct X
{
    char c_;

    X() : c_ {'~'}
    { std::cout << "new  ~\n"; }
    X(char c) : c_ {c}
    { std::cout << "new  " << c_ << '\n'; }
    ~X()
    { std::cout << "del  " << c_ << '\n'; }
    X& operator=(const X& rhs)
    {
        std::cout   << "copy " << rhs.c_
                    << " over " << c_ << '\n';
        c_ = rhs.c_;
        return *this;
    }
    X(const X& rhs) : c_ {rhs.c_}
    { std::cout << "ctor " << c_ << '\n'; }

    const auto addr() const
    { return this; }
};

struct Y
{
    char c_;

    Y() : c_ {'~'}
    { std::cout << "new  ~\n"; }
    Y(char c) : c_ {c}
    { std::cout << "new  " << c_ << '\n'; }
    ~Y()
    { std::cout << "del  " << c_ << '\n'; }
    Y& operator=(const Y& rhs)
    {
        std::cout   << "copy " << rhs.c_
                    << " over " << c_ << '\n';
        c_ = rhs.c_;
        return *this;
    }
    Y(const Y& rhs) : c_ {rhs.c_}
    { std::cout << "ctor " << c_ << '\n'; }
    Y(Y&& rhs) : c_ { std::exchange(rhs.c_, '~') }
    { std::cout << "mtor " << c_ << '\n'; }
    Y& operator=(Y&& rhs)
    {
        std::cout   << "move " << rhs.c_
                    << " over " << c_ << '\n';
        c_ = std::exchange(rhs.c_, '~');
        return *this;
    }

    const auto addr() const
    { return this; }
};

template<typename T>
T ident(T arg)
{
    return arg;
}

int main()
{
    using namespace boost::ut;
    using namespace boost::ut::bdd;

    "object lifetime"_test = [] {
        given("constructors") = [] {
            my::hijack hj(std::cout);
            when("constructed") = [] {
                X xA {'A'};
                X xB {'B'};
                expect(xA.addr() != xB.addr());
            };
            auto actual = hj.release();
            expect(actual ==
                "new  A\n"
                "new  B\n"
                "del  B\n"
                "del  A\n"
            ) << actual;
        };
        given("copy assign") = [] {
            my::hijack hj(std::cout);
            when("copied") = [] {
                X xA {'A'};
                auto xAaddr = xA.addr();
                X xB {'B'};
                auto xBaddr = xB.addr();
                xA = xB;
                expect(xAaddr == xA.addr());
                expect(xBaddr == xB.addr());
                expect(xA.addr() != xB.addr());
            };
            auto actual = hj.release();
            expect(actual ==
                "new  A\n"
                "new  B\n"
                "copy B over A\n"
                "del  B\n"
                "del  B\n"
            ) << actual;
        };
        given("copy constructor") = [] {
            my::hijack hj(std::cout);
            when("constructed") = [] {
                X xA1 {'A'};
                X xA2 {xA1};
                expect(xA1.addr() != xA2.addr());
            };
            auto actual = hj.release();
            expect(actual ==
                "new  A\n"
                "ctor A\n"
                "del  A\n"
                "del  A\n"
            ) << actual;
        };
        given("without move ctor & assign") = [] {
            my::hijack hj(std::cout);
            when("copied") = [] {
                X xA {'A'};
                X xB {'B'};
                auto xAaddr = xA.addr();
                xA = ident(xB);
                expect(xAaddr == xA.addr());
            };
            auto actual = hj.release();
            expect(actual ==
                "new  A\n"
                "new  B\n"
                "ctor B\n"
                "ctor B\n"
                "copy B over A\n"
                "del  B\n"
                "del  B\n"
                "del  B\n"
                "del  B\n"
            ) << actual;
        };
        given("move ctor & assign") = [] {
            my::hijack hj(std::cout);
            when("copied") = [] {
                Y yA {'A'};
                Y yB {'B'};
                auto yAaddr = yA.addr();
                yA = ident(yB);
                expect(yAaddr == yA.addr());
            };
            auto actual = hj.release();
            expect(actual ==
                "new  A\n"
                "new  B\n"
                "ctor B\n"
                "mtor B\n"
                "move B over A\n"
                "del  ~\n"
                "del  ~\n"
                "del  B\n"
                "del  B\n"
            ) << actual;
        };
    };
}
